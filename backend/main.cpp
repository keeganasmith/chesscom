#include "analysis/Stockfish_Wrapper.h"

int main(){
    Chesscom_Client client;
    vector<Game> games = client.retrieve_games("mypasswordisreally");
    Stockfish_Wrapper my_wrapper;
    //Move_Recommendation result = my_wrapper.analyze_move(games[0], games[0].pgn.moves.size());
    //cout << result << "\n";
    vector<Move_Recommendation> result = my_wrapper.analyze_game(games[0]);
    cout << result;
}