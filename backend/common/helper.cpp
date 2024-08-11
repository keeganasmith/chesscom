#include "helper.h"
#include <sstream>
#include <map>
using std::vector, std::string, std::stringstream, std::map;
vector<string> find_strings_enclosed_in(const string& joe, char delimiter){
    vector<string> result;
    string current = "";
    bool appending = false;
    for(int i = 0; i < joe.size(); i++){
        if(joe.at(i) == delimiter && !appending){
            appending = true;
            continue;
        }
        if(joe.at(i) == delimiter){
            appending = false;
            result.push_back(current);
            current = "";
            continue;
        }
        if(appending){
            current.push_back(joe.at(i));
        }

    }
    return result;
    
}
vector<string> split_string(const string& joe, char delimiter){
    stringstream ss(joe);
    vector<string> result;
    string word;
    while(getline(ss, word, delimiter)){
        result.push_back(word);
    }
    return result;
}
int convert_time_to_int(const string& time){
    vector<string> parsed = split_string(time, ':');
    int hours = stoi(parsed[0]);
    int minutes = stoi(parsed[1]);
    int seconds = stoi(parsed[2]);
    return hours * 3600 + minutes * 60 + seconds;
}
string get_string_up_to(const string& joe, char target){
    string result = "";
    for(int i = 0; i < joe.size(); i++){
        if(joe.at(i) == target){
            break;
        }
        result.push_back(joe.at(i));
    }
    return result;
}

bool is_castling(const string& lan){
    if(lan.at(0) == 'O' || lan.at(0) == '0'){
        return true;
    }
    return false;
}
bool is_pawn(const string& lan){
    return lan.at(0) >= 97 && lan.at(0) <= 122; 
}
string retrieve_move_color_fen(const string& fen){
    stringstream ss(fen);
    string move;
    ss >> move;
    ss >> move;
    if(move == "w"){
        return "White";
    }
    else{
        return "Black";
    }
}

string castling_to_lan(const string& fen, const string& move){
    map<string, map<string, string>> castling_map;
    castling_map["White"] = std::map<std::string, std::string>();
    castling_map["Black"] = std::map<std::string, std::string>();
    
    castling_map["White"]["Queen"] = "e1c1";
    castling_map["White"]["King"] = "e1g1";
    castling_map["Black"]["Queen"] = "e8c8";
    castling_map["Black"]["King"] = "e8g8";
    string type = "Queen";
    if(move == "O-O" || move == "0-0"){
        type = "King";
    }
    string color = retrieve_move_color_fen(fen);
    return castling_map[color][type];

}
