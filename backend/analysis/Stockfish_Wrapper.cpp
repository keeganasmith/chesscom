#include "Stockfish_Wrapper.h"
#include <stdexcept>
#include <sstream>
using std::runtime_error, std::to_string, std::stringstream, std::array;
const int num_threads = 12;
Move_Recommendation::Move_Recommendation(){}
crow::json::wvalue Move_Recommendation::to_json(){
    /*
    string best_move;
    double score;
    vector<string> best_line;
    */
    crow::json::wvalue result;
    result["best_move"] = this->best_move;
    result["score"] = this->score;
    result["best_line"] = crow::json::wvalue();
    for(int i = 0;i < this->best_line.size(); i++){
        result["best_line"][i] = this->best_line.at(i);
    }
    return result;
}
Stockfish_Wrapper::Stockfish_Wrapper(): depth(20){
    int to_stockfish[2];
    int from_stockfish[2];
    cout << std::flush;
    if(pipe(to_stockfish) == -1 || pipe(from_stockfish) == -1){
        perror("couldn't create pipes");
        throw runtime_error("couldn't create pipes");
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        throw runtime_error("Unable to fork process");
    }
    if(pid == 0){
        dup2(to_stockfish[0], STDIN_FILENO);
        dup2(from_stockfish[1], STDOUT_FILENO);

        close(to_stockfish[1]);
        close(from_stockfish[0]);

        execlp("stockfish", "stockfish", nullptr);
        perror("execlp");
        exit(1);
    }
    else{
        close(to_stockfish[0]);
        close(from_stockfish[1]);

        this->stockfish_in = to_stockfish[1];
        this->stockfish_out = from_stockfish[0];

        string set_threads_cmd = "setoption name Threads value " + to_string(num_threads) + "\n";
        write_to_stockfish("uci\n");
        write_to_stockfish(set_threads_cmd);
    }
}

Stockfish_Wrapper::~Stockfish_Wrapper(){
    close(this->stockfish_in);
    close(this->stockfish_out);
}
void Stockfish_Wrapper::write_to_stockfish(const string& cmd){
    if (write(stockfish_in, cmd.c_str(), cmd.size()) == -1) {
        perror("write");
        throw runtime_error("Failed to write to Stockfish");
    }
    fsync(stockfish_in);
}
string Stockfish_Wrapper::read_from_stockfish(){
    array<char, BUF_SIZE> buffer;
    string output;
    ssize_t count;
    cout << "stockfish output:\n";
    while ((count = read(stockfish_out, buffer.data(), buffer.size())) > 0) {
        output.append(buffer.data(), count);
        cout << output << "\n";
        if (output.find("bestmove") != string::npos) {
            break;
        }
    }
    return output;
}
Move_Recommendation Stockfish_Wrapper::parse_stockfish_output(const string& stockfish_output){
    stringstream ss(stockfish_output);
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
Move_Recommendation Stockfish_Wrapper::analyze_position(const string& fen){
    string position_cmd = "position fen " + fen + "\n";
    write_to_stockfish(position_cmd);
    
    string go_cmd = "go depth " + to_string(this->depth) + "\n";
    write_to_stockfish(go_cmd);
    string stockfish_output = read_from_stockfish();
    Move_Recommendation recommendation = parse_stockfish_output(stockfish_output);
    return recommendation;
}

Move_Recommendation Stockfish_Wrapper::analyze_move(const Game& my_game, int num_moves){
    string moves_string = "";
    for(int i = 0; i < num_moves; i++){
        moves_string += my_game.pgn.moves.at(i).notation + " ";
    }
    //cout << num_moves << " moves string was: " << moves_string << "\n";
    string position_cmd = "position fen " + my_game.start_pos + " moves " + moves_string + "\n";
    write_to_stockfish(position_cmd);

    string go_cmd = "go depth " + to_string(this->depth) + "\n";
    write_to_stockfish(go_cmd);
    
    string intermediate = read_from_stockfish();

    Move_Recommendation recommendation = parse_stockfish_output(intermediate);
    return recommendation;

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
