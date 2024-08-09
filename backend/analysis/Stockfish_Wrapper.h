#include <string>
using std::string;
class Stockfish_Wrapper{
    public:
    Stockfish_Wrapper();
    double analyze_position(const string& fen);
};