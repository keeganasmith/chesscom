#include "Stockfish_Wrapper.h"
#include <stdexcept>
using std::runtime_error
Stockfish_Wrapper::Stockfish_Wrapper(): buf[BUF_SIZE]{
    this->fp = popen("stockfish", "r+");
    if(fp == nullptr){
        throw runtime_error("Unable to run stockfish");
    }
    fprintf(fp, "uci\n");
    fflush(fp);
}

Stockfish_Wrapper::~Stockfish_Wrapper(){
    pclose(fp);
    delete this->fp;
    delete[] this->buf;
}

