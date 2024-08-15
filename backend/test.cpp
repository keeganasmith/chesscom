#include "analysis/Stockfish_Wrapper.h"
void test(){
    cout << "got here\n";
    Chesscom_Client client;
    cout << "got here2\n";
    vector<Game> games = client.retrieve_games("mypasswordisreally");
    cout << "got here1";
    Stockfish_Wrapper my_wrapper;
    //Move_Recommendation result = my_wrapper.analyze_move(games[0], games[0].pgn.moves.size());
    //cout << result << "\n";
    vector<Move_Recommendation> result = my_wrapper.analyze_game(games[0]);
    for(int i = 1; i < result.size(); i++){
        if(result.at(i).best_move == result.at(i-1).best_move){
            cout << "issue was around move " << i << "\n";
            break;
        }
    }
    cout << result;
}
void test_retrieve_games(){
    Chesscom_Client chess_com_client;
    string username= "mypasswordisreally";
    
    crow::json::wvalue result = chess_com_client.retrieve_games_json(username, 10);
}
int main(){
    //test();
    test_retrieve_games();
}