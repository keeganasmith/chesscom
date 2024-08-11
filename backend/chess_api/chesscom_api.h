#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include "../json.hpp"
#include <string>
#include <iostream>
#include <vector>
using std::string, std::cout, std::vector;
const string url = "https://api.chess.com";
struct Move{
    string notation;
    double time_taken;
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
    string time_control;
    vector<Move> moves;
    PGN();
    PGN(const string& pgn);
    void construct_from_string(const string& pgn);
};
struct Game{
    string start_pos;
    vector<Move> moves;
    Game();
    Game(const string& start_pos);
};
class Chesscom_Client{
    httplib::Client cli;
    
    public:
    Chesscom_Client();
    void retrieve_games(const string& user);
    PGN parse_pgn(const string& pgn);
};
std::ostream& operator<<(std::ostream& os, const Move& myMove);
std::ostream& operator<<(std::ostream& os, const Game& myGame);