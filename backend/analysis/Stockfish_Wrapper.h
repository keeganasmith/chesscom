#include <string>
using std::string;
const int BUF_SIZE=1024;

class Stockfish_Wrapper{
    FILE* fp;
    char* buf;
    public:
    Stockfish_Wrapper();
    ~Stockfish_Wrapper();
    double analyze_position(const string& fen);
};