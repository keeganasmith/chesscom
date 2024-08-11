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
            this->time = int_time;
        }
        else if(key == "WhiteElo"){
            int white_elo = stoi(pars[0]);
            this->white_elo = white_elo;
        }
        else if(key == "BlackElo"){
            int black_elo = stoi(pars[0]);
            this->black_elo = black_elo;
        }
        else if(key == "1."){
            this->moves = this->get_moves_from_string(curr_line);
        }
    }
}
vector<Move> PGN::get_moves_from_string(const string& moves){
    int i = 0;
    stringstream ss(moves);
    string curr_word;
    vector<Move> result;
    while(ss >> curr_word){
        while(true){
            if(curr_word.find('.') == string::npos){
                break;
            }
            int move_num = stoi(get_string_up_to(curr_word, '.'));
            ss >> curr_word;
            Move my_move;
            my_move.notation = curr_word;
            ss >> curr_word;
            ss >> curr_word;
            curr_word.erase(curr_word.size()-2, 2);
            vector<string> split = split_string(curr_word, ':');
            int hours = stoi(split[0]);
            int minutes = stoi(split[1]);
            double seconds = stod(split[2]);
            double total_seconds = (double)hours * 3600.0 + (double)minutes * 60.0 + seconds;
            my_move.clock_time = total_seconds;
            result.push_back(my_move);
            ss >> curr_word;
        }
    }
    return result;
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
    
};
std::ostream& operator<<(std::ostream& os, const Move& myMove) {
    os  << "Move: {\n"<< "notation: " << myMove.notation << "\n" << "time: " << myMove.clock_time << "\n}\n";
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