#ifndef GAME_H
#define GAME_H

#include<cstdio>
#include<cstdlib>
#include<ncurses.h>
#include<menu.h> //for ncurses
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

#include "../ncurses/logger.h"

int main(int argc, const char* argv[]);

#define len(a) ( sizeof(a) / sizeof(a[0]) )

//haxx to make ncurses init properly else weird segfaults, TODO varför funkar inte dethär ??
#define INIT_NCURSES (initscr();cbreak();noecho();keypad(stdscr,TRUE);)

namespace hax{
    extern Logger log;

    class Level; //forward declaration
    std::vector<std::string> split(std::string);
    bool commandMode;

    int welcome_menu(); //displays the start menu returns the index of the item clicked(enter)
    void credits();
    void newGame();
    Level* initNewGame();
    void loadGame();
    void options();

}
#endif
