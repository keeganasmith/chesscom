#include "chesscom_api.h"
#include <ctime>
#include <sstream>
#include "../common/helper.h"
using std::time_t, std::tm, std::time, std::localtime, std::to_string, std::string, std::stringstream;
using json = nlohmann::json;
PGN::PGN(){}
PGN::PGN(const string& pgn){
    this->construct_from_string(pgn);
}
void PGN::construct_from_string(const string& pgn){
    stringstream ss(pgn);
    string curr_line;
    while(getline(ss, curr_line)){
        if(curr_line == ""){
            continue;
        }
        int start_index = 0;
        int end_index = curr_line.size() -1;
        if(curr_line.at(0) == '['){
            curr_line.erase(0, 1);
        }
        if(curr_line.at(curr_line.size()-1) == ']'){
            curr_line.erase(curr_line.size()-1, 1);
        }
        cout << curr_line << "\n";
        stringstream ss(curr_line);
        string key;
        ss >> key;
        vector<string> pars = find_strings_enclosed_in(curr_line, '"');
        if(key == "Event"){
            this->event = pars[0];
        }
        else if(key == "Site"){
            this->site = pars[0];
        }
        else if(key == "Date"){
            this->date = pars[0];
        }
        else if(key == "White"){
            this->white_player = pars[0];
        }
        else if(key == "Black"){
            this->black_player = pars[0];
        }
        else if(key == "UTCTime"){
            string pretty_time = pars[0];
            int int_time = convert_time_to_int(pretty_time);
            //cout << int_time << "\n";
        }
    }
}
Game::Game(){}
Game::Game(const string& start_pos) {
    this->start_pos = start_pos;
}


Chesscom_Client::Chesscom_Client(): cli(url){
}

void Chesscom_Client::retrieve_games(const string& user){
    string game_endpoint = "/pub/player/" + user + "/games/";

    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int current_month = now->tm_mon + 1;
    string curr_month = to_string(current_month);
    if(curr_month.size() < 2){
        curr_month = "0" + curr_month;
    }
    int current_year = now->tm_year + 1900;
    string curr_year = to_string(current_year);

    game_endpoint += curr_year + "/" + curr_month;
    auto result = Chesscom_Client::cli.Get(game_endpoint);
    json my_json = json::parse(result->body);
    for(int i = 0; i < my_json["games"].size(); i++){
        PGN my_pgn(my_json["games"][i]["pgn"]);
    }
    //cout << my_json["games"][0]["pgn"] << "\n";
    
};
std::ostream& operator<<(std::ostream& os, const Move& myMove) {
    os  << "Move: {\n"<< "notation: " << myMove.notation << "\n" << "time: " << myMove.time_taken << "\n}\n";
    return os;
}
std::ostream& operator<<(std::ostream& os, const Game& myGame) {
    os  << "Game: { \n"<< "start_pos: " << myGame.start_pos << "\n" << "moves: [";
    for(int i = 0; i < myGame.moves.size()-1; i++){
        os << myGame.moves.at(i) << ", ";
    }
    os << myGame.moves.at(myGame.moves.size()-1) << "]\n}\n";
    return os;
}