#include "chesscom_api.h"
#include <ctime>
using std::time_t, std::tm, std::time, std::localtime, std::to_string, std::string;

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
    cout << result->body << "\n";
    
};