#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include "../json.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <crow.h>
using std::string, std::cout, std::vector, std::map;
using json = nlohmann::json;

const string url = "https://api.chess.com";

struct Move_LAN{
    string notation;
    double clock_time;
    crow::json::wvalue to_json();
};
struct PGN{

    string event;
    string site;
    string date;
    string white_player;
    string black_player;
    int time;
    int white_elo;
    int black_elo;
    int total_time;
    int increment; 
    vector<Move_LAN> moves;
    PGN();
    PGN(const string& pgn, const string& initial_fen);
    PGN(const crow::json::wvalue& my_json);
    void construct_from_string(const string& pgn, const string& initial_fen);
    vector<Move_LAN> get_moves_from_string(const string& moves, const string& initial_fen);
    crow::json::wvalue to_json();
};
struct Game{
    string start_pos;
    PGN pgn;
    string black_result;
    string white_result;
    Game();
    Game(const string& start_pos, const string& pgn_string);
    Game(const json& chess_com_json);
    Game(const crow::json::wvalue& my_json);
    crow::json::wvalue to_json();
};
class Chesscom_Client{
    httplib::Client cli;
    
    public:
    Chesscom_Client();
    vector<Game> retrieve_games(const string& user);
    vector<Game> retrieve_games(const string& user, int num_games_to_retrieve);
    crow::json::wvalue retrieve_games_json(const string& user);
    crow::json::wvalue retrieve_games_json(const string& user, int num_games_to_retrieve);
};
std::ostream& operator<<(std::ostream& os, const Move_LAN& myMove);
std::ostream& operator<<(std::ostream& os, const Game& myGame);
std::ostream& operator<<(std::ostream& os, const PGN& my_pgn);