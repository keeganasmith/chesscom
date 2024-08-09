#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include <string>
#include <iostream>
using namespace std;
const string url = "https://api.chess.com";

httplib::Client cli(url);

int main(){
    const string game_endpoint = url + "/pub/player/mypasswordisreally/games/2024/07";
    auto result = cli.Get(game_endpoint);
    cout << result->body << "\n";
}
