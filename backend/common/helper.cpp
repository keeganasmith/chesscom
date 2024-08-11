#include "helper.h"
#include <sstream>
using std::vector, std::string, std::stringstream;
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

