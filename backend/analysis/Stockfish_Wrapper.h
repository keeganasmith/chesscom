#include <string>
#include "../chess_api/chesscom_api.h"
#include "../common/helper.h"

using std::string;
const int BUF_SIZE=1024;
struct Move_Recommendation{
    string best_move;
    double score;
    vector<string> best_line;
    Move_Recommendation();
};
class Stockfish_Wrapper{
    FILE* fp;
    int depth;
    public:

    Stockfish_Wrapper();
    ~Stockfish_Wrapper();
    Move_Recommendation analyze_move(const Game& my_game, int num_moves);
    vector<Move_Recommendation> analyze_game(const Game& my_game);
};
std::ostream& operator<<(std::ostream& os, const Move_Recommendation& my_recommendation);
