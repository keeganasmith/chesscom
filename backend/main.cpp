#include <crow.h>
#include "crow/middlewares/cors.h"
#include "analysis/Stockfish_Wrapper.h"
using std::cerr;
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
    CROW_ROUTE(app, "/analyze_position")
    .methods("POST"_method)
    ([](const crow::request& req){
        auto json_sent = crow::json::load(req.body);
        if(!json_sent){
            return crow::response(crow::status::BAD_REQUEST);
        }
        string fen = json_sent["fen"].s();
        Stockfish_Wrapper joe;
        Move_Recommendation recommendation = joe.analyze_position(fen);
        return crow::response{recommendation.to_json()};

    });
    app.port(18080).multithreaded().run();
}