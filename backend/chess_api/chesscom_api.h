#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include <string>
#include <iostream>
using std::string, std::cout;
const string url = "https://api.chess.com";

class Chesscom_Client{
    httplib::Client cli;
    
    public:
    Chesscom_Client();
    void retrieve_games(const string& user);
};
