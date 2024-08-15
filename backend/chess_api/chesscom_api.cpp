#include "chesscom_api.h"
#include <ctime>
#include <sstream>
#include "../common/helper.h"
using std::time_t, std::tm, std::time, std::localtime, std::to_string, std::string, std::stringstream;
using json = nlohmann::json;
using chess::Board, chess::Move, chess::uci;
crow::json::wvalue Move_LAN::to_json(){
    crow::json::wvalue result;
    result["notation"] = this->notation;
    result["clock_time"] = this->clock_time;
    return result;
}
PGN::PGN(){}
PGN::PGN(const string& pgn, const string& initial_fen){
    
    this->construct_from_string(pgn, initial_fen);
}
crow::json::wvalue PGN::to_json(){
    crow::json::wvalue result;
    result["event"] = this->event;
    result["site"] = this->site;
    result["date"] = this->date;
    result["white_player"] = this->white_player;
    result["black_player"] = this->black_player;
    result["time"] = this->time;
    result["white_elo"] = this->white_elo;
    result["black_elo"] = this->black_elo;
    result["total_time"] = this->total_time;
    result["increment"] = this->increment;
    result["moves"] = crow::json::wvalue();
    for(int i = 0; i < this->moves.size(); i++){
        result["moves"][i] = this->moves.at(i).to_json();
    }
    return result;
}
void PGN::construct_from_string(const string& pgn, const string& initial_fen){
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
        else if(key == "TimeControl"){
            vector<string> parsed = split_string(pars[0], '+');
            this->total_time = stoi(parsed[0]);
            if(parsed.size() == 2){
                this->increment = stoi(parsed[1]);
            }
            else{
                this->increment = 0;
            }
        }
        else if(key == "1."){
            this->moves = this->get_moves_from_string(curr_line, initial_fen);
        }
    }
}
vector<Move_LAN> PGN::get_moves_from_string(const string& moves, const string& initial_fen){
    int i = 0;
    stringstream ss(moves);
    string curr_word;
    vector<Move_LAN> result;
    string current_fen = initial_fen;
    Board my_board(current_fen);
    while(ss >> curr_word){
        while(true){
            if(curr_word.find('.') == string::npos){
                break;
            }
            int move_num = stoi(get_string_up_to(curr_word, '.'));
            ss >> curr_word;
            Move_LAN my_move;
            Move chess_move = uci::parseSan(my_board, curr_word);
            my_move.notation = uci::moveToUci(chess_move);
            // if(!(is_castling(my_move.notation) || is_pawn(my_move.notation)) ){
            //     my_move.notation.erase(0, 1);
            // }
            // if(is_castling(my_move.notation)){
            //     my_move.notation = castling_to_lan(current_fen, my_move.notation);
            // }
            my_board.makeMove(chess_move);
            //current_fen = my_board.getFen();
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
Game::Game(const string& start_pos, const string& pgn_string) {
    this->start_pos = start_pos;
    PGN my_pgn(pgn_string, start_pos);
    this->pgn = my_pgn;
}
Game::Game(const json& chess_com_json){
    this->start_pos = chess_com_json["initial_setup"];
    PGN my_pgn(chess_com_json["pgn"], this->start_pos);
    this->pgn = my_pgn;
    this->black_result = chess_com_json["black"]["result"];
    this->white_result = chess_com_json["white"]["result"];
}
crow::json::wvalue Game::to_json(){
    crow::json::wvalue result;
    result["start_pos"] = this->start_pos;
    result["pgn"] = this->pgn.to_json();
    result["black_result"] = this->black_result;
    result["white_result"] = this->white_result;
    return result;
}

Chesscom_Client::Chesscom_Client(): cli(url){
}

vector<Game> Chesscom_Client::retrieve_games(const string& user){
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
    auto api_result = Chesscom_Client::cli.Get(game_endpoint);
    json my_json = json::parse(api_result->body);
    vector<Game> result;
    for(int i = 0; i < my_json["games"].size(); i++){
        Game my_game(my_json["games"][i]);
        result.push_back(my_game);
    }
    cout << "reached end of chesscum client\n";
    return result;
};
crow::json::wvalue Chesscom_Client::retrieve_games_json(const string& user){
    vector<Game> raw_result = this->retrieve_games(user);
    crow::json::wvalue result;
    for(int i =0; i < raw_result.size(); i++){
        result[i] = raw_result.at(i).to_json();
    }
    return result;
}
vector<Game> Chesscom_Client::retrieve_games(const string& user, int num_games_to_retrieve){
    string archive_endpoint = "/pub/player/" + user + "/games/archives";
    auto api_result = Chesscom_Client::cli.Get(archive_endpoint);
    json archive_json = json::parse(api_result->body);
    vector<string> game_archive_urls = archive_json["archives"];
    vector<Game> result; 
    for(int i = game_archive_urls.size()-1; i >= 0; i--){
        game_archive_urls.at(i).erase(0, 21);
        api_result = Chesscom_Client::cli.Get(game_archive_urls.at(i));
        json game_archive_json = json::parse(api_result->body);
        for(int i = game_archive_json["games"].size()-1; i >= 0; i--){
            cout << "game json looks like: " << game_archive_json["games"][i] << "\n\n";
            Game my_game(game_archive_json["games"][i]);
            result.push_back(my_game);
            if(result.size() >= num_games_to_retrieve){
                break;
            }
        }
        if(result.size() >= num_games_to_retrieve){
            break;
        }
    }
    cout << "size of result was: " << result.size() << "\n";
    return result;
}
crow::json::wvalue Chesscom_Client::retrieve_games_json(const string& user, int num_games_to_retrieve){
    vector<Game> raw_result = this->retrieve_games(user, num_games_to_retrieve);
    crow::json::wvalue result;
    for(int i =0; i < raw_result.size(); i++){
        
        result[i] = raw_result.at(i).to_json();
    }
    return result;
}
std::ostream& operator<<(std::ostream& os, const Move_LAN& myMove) {
    
    os  << "Move: {\n"<< "notation: " << myMove.notation << "\n" << "time: " << myMove.clock_time << "\n}\n";
    return os;
}
std::ostream& operator<<(std::ostream& os, const PGN& my_pgn){
    os << "PGN: {\n" << "event: " << my_pgn.event << "\nsite: " << my_pgn.site << "\ndate: "<< my_pgn.date << "\nwhite_player: " << my_pgn.white_player << "\nblack_player: " << my_pgn.black_player << "\ntime: " << my_pgn.time << "\nwhite_elo: " << my_pgn.white_elo << "\nblack_elo: " << my_pgn.black_elo << "\ntotal_time: " << my_pgn.total_time << "\nincrement: " << my_pgn.increment << "\nmoves: [";
    for(int i = 0; i < my_pgn.moves.size()-1; i++){
        os << my_pgn.moves.at(i) << ", ";
    }
    os << my_pgn.moves.at(my_pgn.moves.size()-1) << "]\n}\n";
    return os; 
}
std::ostream& operator<<(std::ostream& os, const Game& myGame) {
    os  << "Game: { \n"<< "start_pos: " << myGame.start_pos << "\n" << myGame.pgn << "\n}\n";
    return os;
}