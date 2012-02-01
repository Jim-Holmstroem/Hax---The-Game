#include "game.h"
#include "level.h"
#include "../area/indoor.h"
#include "../area/outdoor.h"
#include "../ch/human.h"
#include "../ch/undead.h"
#include "../obj/container.h"
#include "../obj/obstacle.h"
#include "../area/road.h"
#include "../ncurses/sprite.h"
#include "../ncurses/multisprite.h"

/*
*   Loads a char * filename into namespace img
*   To retrieve img::filename
*/
#include "../ncurses/cat1.asciiart"

void msleep(unsigned long milisec){
    struct timespec req={0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec = sec;
    req.tv_nsec=milisec*1000000L;
    while(nanosleep(&req,&req)==-1){
        continue;
    }
    return;
};

int main(int argc, const char* argv[]){

    //DEBUGGING

    initscr();
    cbreak();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);

    int opt = 0;

    while((opt = hax::welcome_menu())!=4){
        switch(opt){
            case 0:
                hax::newGame();
                break;
            case 1:
                mvprintw(LINES-4,0,"not supported yet");
                break;
            case 2:
                mvprintw(LINES-3,0,"not supported yet");
                break;
            case 3:
                hax::credits();
                break;
            default:
                break;
        }
        refresh();
    }


    endwin();

    printf("0x0%x\n",opt);

    return 0;
}; //end of main

void hax::options(){

};

std::vector<std::string> hax::split(std::string input){
    std::vector<std::string> words;

    std::istringstream iss(input);
    do{
        std::string word;
        iss >> word; //read a word
        words.push_back(word); //add the word to words
    }while(iss);//while not empty

    words.pop_back(); //delete the ENTER that followed to input

    return words;
};

hax::Level* hax::initNewGame(){ //use auto pointer?
    //create objects
    Level* test = new Level();
    Area* borg = new Castle("Hogwarts");
    Area* kth = new School("KTH");
    Area* skog = new Forest("FoReSt Of DoOm");

    Character* albus = new Wizard("Albus");
    Character* voldy = new Wizard("Voldy");
    Character* conan = new Barbarian("Conan");
    Character* snape = new Wizard("Snape");
    Character* necro = new Necromancer("Sauron");

    Key* nyckel = new Key();

    //build level
    test->add(borg);
    test->add(kth);
    test->add(skog);
    test->add(albus);
    test->add(voldy);
    test->add(conan);
    test->add(necro);
    borg->addRoute(new Road("north", borg, kth));
    borg->addRoute(new Road("west", borg, skog));
    kth->addRoute(new Road("south", kth, borg));
    kth->addRoute(new Door("southwest", kth, skog, nyckel));
    skog->addRoute(new Road("east", skog, borg, new Tree()));
    skog->addRoute(new Road("northeast", skog, kth, snape));

    //add Character to Area
    borg->enter(albus);
    borg->enter(necro);
    kth->enter(conan);
    kth->enter(voldy);

    //add Object to Area
    kth->pick_up(nyckel);

    //add Object to Character

    return test;
};

void hax::newGame(){
    std::auto_ptr<Level> myGame( initNewGame() ); //I want a fixed pointer that cannot change to another object but can manipulate its object!!!!!
//    std::for_each(myGame->allChars().begin(), myGame->allChars().end(), std::mem_fun(&Character::action)); //REMEMBER very bad to use iterator on two copies of same vector

    int c = 0;

    curs_set(0);//set invisb cursor
    nodelay(stdscr,true);

    Sprite  s(3,3,'0');

    Sprite s1(5,5,'x');
    Sprite s2(5,6,'x');
    Sprite s3(5,7,'x');

    Sprite s4(6,7,'.');
    Sprite s5(7,7,',');
    Sprite s6(8,7,'.');

    MultiSprite ms;
    ms.addSprite(&s1);
    ms.addSprite(&s2);
    ms.addSprite(&s3);

    MultiSprite ms2;
    ms2.addSprite(&s4);
    ms2.addSprite(&s5);
    ms2.addSprite(&s6);

    ms.addSprite(&ms2);

    hax::log.write("Type help for a list of available commands. Pay attention to case sensitivity!");

    clock_t timer;
    while(true){;

        if(!commandMode){

            timer = clock();
            c = getch();

            s.drawClear();
            ms.drawClear();

            refresh();
            switch(c){
                case KEY_LEFT:
                    s.moveDirection(DIR_LEFT);
                    break;
                case KEY_RIGHT:
                    s.moveDirection(DIR_RIGHT);
                    break;
                case KEY_UP:
                    s.moveDirection(DIR_UP);
                    break;
                case KEY_DOWN:
                    s.moveDirection(DIR_DOWN);
                    break;
                case ERR: //no input

                    break;
                case 'q':
                    return;
                    break;
                case 'n':
                    s.setPosition(1,1);
                    break;
                case 'p':
                    ms.setPosition(0,0);
                    break;
                case 'l':
                    ms.move(1,1);
                    break;

                case 'w':
                    ms.moveDirection(DIR_UP);
                    break;
                case 's':
                    ms.moveDirection(DIR_DOWN);
                    break;
                case 'a':
                    ms.moveDirection(DIR_LEFT);
                    break;
                case 'd':
                    ms.moveDirection(DIR_RIGHT);
                    break;;
                case 10:
                    commandMode = true;
                    break;
                default:
                    //mvprintw(0,0,"0x0%x",c);

                    break;
            }


            //update


            ms.draw();
            s.draw();
            refresh();

            //time in (about 12.5fps)
            int sleeptime = 80;//-(clock()-timer)/(1000*CLOCKS_PER_MS);

            if(sleeptime>0){
                msleep(sleeptime);
            }
        }else{

            char input[512];
            echo();
            nodelay(stdscr,false);

//TODO nicer blank-row
            mvprintw(LINES-1,0,"                                                                                                                                                                                                                               ");
            mvprintw(LINES-1,0,"Command: ");

            //mvscanw(LINES-1,9,"%s",input);
            getstr(input);

            //TODO go back to game
            noecho();
            nodelay(stdscr,true);
            commandMode = false;

            myGame->parse( hax::split(input) );
        }

    }

}; //end of newGame

void hax::loadGame(){
    mvprintw( 2, 0, "loadGame()");
    refresh();
};

void hax::credits(){
    mvprintw( 2, 2, "CREDITS");
    mvprintw( 5, 2, "Jim Holmström, F-08");
    refresh();
    sleep(1);
    mvprintw( 5, 2, "   Simon Ye, F-08  ");
    refresh();
    sleep(2);
};

int hax::welcome_menu(){

    char *choices[] = {
        "New Game",
        "Load Game",
        "Options",
        "Credz",
        "Exit",
    };

    int c=0;
    MENU *start_menu;
    int n_choices;

    mvprintw(0,0,img::cat1);

    n_choices = len(choices);
    ITEM *items[n_choices+1];

    for(int i=0;i<n_choices;++i){
        items[i] = new_item(choices[i]," ");
    }

    items[n_choices] = NULL;

    start_menu = new_menu(items); //<--------

    mvprintw(LINES - 2, 0, "q to exit");
    post_menu(start_menu);

    refresh();

    while(true){
        c = getch();
        int value = 0;
        switch(c){
            case KEY_DOWN:
                menu_driver(start_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(start_menu, REQ_UP_ITEM);
                break;
            case 10: //enter
                //save the index so it could be returned
                value =  item_index(current_item(start_menu));
                //free memory
                unpost_menu(start_menu); //make invisible
                free_menu(start_menu);
                for(int i = 0; i < n_choices; ++i){
                    free_item(items[i]);
                }
                refresh();
                return value;
                break;
            case 'q':
                //free memory
                unpost_menu(start_menu); //make invisible
                free_menu(start_menu);
                for(int i = 0; i < n_choices; ++i){
                    free_item(items[i]);
                }
                refresh();
                return 4;
                break;
            default:
                break;
        }
    }
}; //end of welcome_menu
