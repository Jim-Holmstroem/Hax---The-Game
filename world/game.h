#ifndef GAME_H
#define GAME_H

#include<cstdio>
#include<cstdlib>
#include<ncurses.h>
#include<menu.h> //for ncurses
#include<unistd.h> //for sleep(int)
#include<ctime>
#include<csignal>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<memory> //for auto_ptr

#include "../ncurses/logger.h"
#include "../serialize/iserializable.h"

int main(int argc, const char* argv[]);

#define len(a) ( sizeof(a) / sizeof(a[0]) )

//haxx to make ncurses init properly else weird segfaults, TODO varför funkar inte dethär ??
#define INIT_NCURSES (initscr();cbreak();noecho();keypad(stdscr,TRUE);)

namespace hax
{
    extern Logger log;
    extern std::map<std::string,ISerializable*> pointerTable;

    class Level; //forward declaration
    bool commandMode;

    int welcome_menu(); //displays the start menu returns the index of the item clicked(enter)
    void credits();
    void newGame();
    Level* initNewGame();
    void loadGame();
    void options();
}
#endif
