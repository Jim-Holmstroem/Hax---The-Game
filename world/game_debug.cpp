#include "game_debug.h"
#include "level.h"
#include "../area/indoor.h"
#include "../area/outdoor.h"
#include "../ch/human.h"
#include "../ch/undead.h"
#include "../obj/container.h"
#include "../obj/obstacle.h"
#include "../area/road.h"
#include "../helper.h"

namespace hax
{
    /*Global variables*/
    std::queue<ISerializable*> serializeQueue; //used for saving game state to file
    std::map<std::string,ISerializable*> pointerTable; //used for loading game state from file
}

int main(int argc, const char* argv[])
{
    srand(time(NULL)); //initialize random seed to a value based on the time the program is executed

    std::ofstream dbg;
    dbg.open("parse_debug.dat", std::ios::out | std::ios::trunc); //if file existed before it will be overwritten
    dbg << "**New game**\n";
    dbg.close();

    hax::Level* test = hax::initNewGame();
    std::cout << "Type help for a list of available commands." << std::endl;
    std::cout << "Pay attention to case sensitivity!" << std::endl;

    std::string input = "";
    while(input != "quit")
    {
        std::cout << ">> ";
        std::getline(std::cin, input);
        if(input != "")
        {
            std::ofstream dbg;
            dbg.open("parse_debug.dat", std::ios::out | std::ios::app); //append to EOF
            dbg << input << std::endl; //write to parse_debug.dat
            dbg.close();
            test->parse(hax::split(input));
        }
    }
    delete test;

    return 0;
};

hax::Level* hax::initNewGame()
{
    //create objects
    Level* test = new Level();
    Area* borg = new Castle("Hogwarts");
    Area* kth = new School("KTH");
    Area* skog = new Forest("FoReSt Of DoOm");
    Area* tysk = new Shop("IKEA");

    Character* albus = new Wizard("Albus",1);
    Character* voldy = new Wizard("Voldy",1);
    Character* conan = new Barbarian("Conan",1);
    Character* snape = new Wizard("Snape",0);
    Character* necro = new Necromancer("Sauron",1);

    Key* nyckel = new Key();
    Container* sack = new Backpack();
    Key* alohomora = new Key();

    //build level
    test->add(borg);
    test->add(kth);
    test->add(skog);
    test->add(tysk);
    test->add(albus);
    test->add(voldy);
    test->add(conan);
    test->add(necro);
    borg->addRoute(new Road("north", borg, kth));
    borg->addRoute(new Road("west", borg, skog));
    kth->addRoute(new Road("south", kth, borg));
    kth->addRoute(new Door("southwest", kth, skog, nyckel));
    kth->addRoute(new Door("in", kth, tysk));
    skog->addRoute(new Road("east", skog, borg, new Tree()));
    skog->addRoute(new Road("northeast", skog, kth, snape));
    tysk->addRoute(new Door("out", tysk, kth));

    //add Character to Area
    borg->enter(albus);
    borg->enter(necro);
    kth->enter(conan);
    kth->enter(voldy);

    //add Object to Area
    kth->pick_up(nyckel);
    borg->pick_up(sack);

    //add Object to Character
    albus->give(alohomora);

    return test;
};
