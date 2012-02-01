#ifndef GAME_DEBUG_H
#define GAME_DEBUG_H

#include<cstdio>
#include<cstdlib>
#include<unistd.h> //for sleep(int)
#include<ctime>
#include<csignal>
#include<cstring>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<memory> //for auto_ptr

int main(int argc, const char* argv[]);

#define len(a) ( sizeof(a) / sizeof(a[0]) )

namespace hax{
    class Level; //forward declaration
    std::vector<std::string>& split(const std::string&, char, std::vector<std::string>&);
    std::vector<std::string> split(const std::string, char);
    Level* initNewGame();
}
#endif
