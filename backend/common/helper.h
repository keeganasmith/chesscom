#include <vector>
#include <string>
#include <iostream>
#include "chess.hpp"
using std::string, std::vector;
vector<string> find_strings_enclosed_in(const string& joe, char delimiter);
int convert_time_to_int(const string& time);
vector<string> split_string(const string& joe, char delimiter);
string get_string_up_to(const string& joe, char target);
template <typename T>
std::ostream& operator<<(std::ostream& os, const vector<T>& my_vector){
    if(my_vector.size() == 0){
        os << "[]";
        return os;
    }
    os << "[";
    for(int i = 0; i < my_vector.size()-1; i++){
        os << my_vector.at(i) << ", ";
    }
    os << my_vector.at(my_vector.size()-1) << "]\n";
    return os;
}
bool is_castling(const string& lan);
bool is_pawn(const string& lan);
string retrieve_move_color_fen(const string& fen);
string castling_to_lan(const string& fen, const string& move);