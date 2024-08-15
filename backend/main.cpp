#include <crow.h>
#include "crow/middlewares/cors.h"
#include "analysis/Stockfish_Wrapper.h"
int main(){
    crow::App<crow::CORSHandler> app;
    Chesscom_Client chess_com_client;
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });
    CROW_ROUTE(app, "/retrieve_games/<string>/<int>")
    ([&chess_com_client](string username, int num_games){
        crow::json::wvalue result = chess_com_client.retrieve_games_json(username, num_games);
        return result;
    });
    app.port(18080).multithreaded().run();
}