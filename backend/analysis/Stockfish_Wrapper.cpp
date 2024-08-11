#include "Stockfish_Wrapper.h"
#include <stdexcept>
#include <sstream>
using std::runtime_error, std::to_string, std::stringstream;
const int num_threads = 12;
Move_Recommendation::Move_Recommendation(){}
Stockfish_Wrapper::Stockfish_Wrapper(): depth(18){
    this->fp = popen("stockfish", "r+");
    if(fp == nullptr){
        throw runtime_error("Unable to run stockfish");
    }
    string set_threads_cmd = "setoption name Threads value " + to_string(num_threads) + "\n";
    fprintf(fp, "uci\n");
    fprintf(fp, "%s", set_threads_cmd.c_str());
    fflush(fp);
}

Stockfish_Wrapper::~Stockfish_Wrapper(){
    pclose(fp);
}

Move_Recommendation Stockfish_Wrapper::analyze_move(const Game& my_game, int num_moves){
    string moves_string = "";
    for(int i = 0; i < num_moves; i++){
        moves_string += my_game.pgn.moves.at(i).notation + " ";
    }
    string position_cmd = "position startpos moves " + moves_string + "\n";
    fprintf(fp, "%s", position_cmd.c_str());
    string go_cmd = "go depth " + to_string(this->depth) + "\n";
    fprintf(fp, "%s", go_cmd.c_str());
    fflush(fp);
    std::array<char, BUF_SIZE> buffer;
    std::string intermediate;
    while (fgets(buffer.data(), buffer.size(), fp) != nullptr) {
        intermediate += buffer.data();

        if(intermediate.find("bestmove") != string::npos){
            break;
        }
    }
    stringstream ss(intermediate);
    string curr_line;
    vector<string> lines;
    while(getline(ss, curr_line)){
        lines.push_back(curr_line);
    }
    Move_Recommendation result;
    vector<string> best_move_info = split_string(lines.at(lines.size()-2), ' ');
    for(int i = 0; i < best_move_info.size(); i++){
        if(best_move_info.at(i) == "score"){
            result.score = stod(best_move_info.at(i + 2)) / 100.0; 
        }
        if(best_move_info.at(i) == "pv"){
            for(int j = i + 1; j < best_move_info.size(); j++){
                result.best_line.push_back(best_move_info.at(j));
            }
        }   
    }
    vector<string> best_move_split = split_string(lines.at(lines.size()-1), ' ');
    result.best_move = best_move_split.at(1);
    return result;

}

vector<Move_Recommendation> Stockfish_Wrapper::analyze_game(const Game& my_game){
    vector<Move_Recommendation> result;
    for(int i = 0; i < my_game.pgn.moves.size(); i++){
        result.emplace_back(analyze_move(my_game, i));
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Move_Recommendation& my_recommendation){
    os << "Move rec: {\nBest move: " << my_recommendation.best_move << "\nScore: " << my_recommendation.score << "\nBest line: ";
    for(int i = 0; i < my_recommendation.best_line.size(); i++){
        os << my_recommendation.best_line.at(i) << " ";
    }
    os << "\n}\n";
    return os;
}
