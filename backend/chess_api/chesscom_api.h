#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include "../json.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <map>
using std::string, std::cout, std::vector, std::map;
const string url = "https://api.chess.com";

struct Move_LAN{
    string notation;
    double clock_time;
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
    void construct_from_string(const string& pgn, const string& initial_fen);
    vector<Move_LAN> get_moves_from_string(const string& moves, const string& initial_fen);
};
struct Game{
    string start_pos;
    PGN pgn;
    Game();
    Game(const string& start_pos, const string& pgn_string);
};
class Chesscom_Client{
    httplib::Client cli;
    
    public:
    Chesscom_Client();
    vector<Game> retrieve_games(const string& user);
    PGN parse_pgn(const string& pgn);
};
std::ostream& operator<<(std::ostream& os, const Move_LAN& myMove);
std::ostream& operator<<(std::ostream& os, const Game& myGame);
std::ostream& operator<<(std::ostream& os, const PGN& my_pgn);