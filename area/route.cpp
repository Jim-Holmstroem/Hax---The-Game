#include<cstdio>
#include<cstdlib>
#include<fstream>
#include "route.h"
#include "../ch/character.h"
#include "../area/area.h"
#include "../obj/container.h" //needed for inventory
#include "../helper.h"

hax::Route::Route()
{
    thisArea = NULL;
    nextArea = NULL;
}
hax::Route::Route(std::string name)
{
    this->name = name;
}
hax::Route::Route(std::string name, Area* from, Area* to)
{
    this->name = name;
    thisArea = from;
    nextArea = to;
}
hax::Route::~Route()
{
    std::cout << name << " route deleted" << std::endl;
}
std::string hax::Route::getName() const{return name;}
bool hax::Route::isBlocked(Character* const ch) const{return false;}
std::string hax::Route::blockMessage() const
{
    return "You are blocked. Bazinga!";
}
std::string hax::Route::passMessage() const
{
    return "You pass through the chosen route.";
}
void hax::Route::ToString(std::ostream& out) const
{
    out << this <<":"<< getType() <<":"<< name <<":"<< view <<":"<< thisArea <<":"<< nextArea <<":";
}
void hax::Route::FromString(std::istream& in)
{
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Route::FromString" << std::endl;

    std::string data;
    std::getline(in,data,':'); //read type
    if(data != getType())
    {
        dbg << "Type mismatch! Aborting load from file." << std::endl;
        dbg.close();
        return;
    }
    dbg << "Type = " << data << std::endl;

    std::getline(in,data,':');
    name = data;
    dbg << "Name = " << name << std::endl;

    std::getline(in,data,':');
    view = data;
    dbg << "View = " << view << std::endl;

    std::getline(in,data,':');
    std::string thisAreaUID = data;
    thisArea = dynamic_cast<Area*>(pointerTable[thisAreaUID]);
    dbg << "thisArea UID = " << thisAreaUID << " | thisArea new address = " << thisArea << std::endl;

    std::getline(in,data,':');
    std::string nextAreaUID = data;
    nextArea = dynamic_cast<Area*>(pointerTable[nextAreaUID]);
    dbg << "nextArea UID = " << nextAreaUID << " | nextArea new address = " << nextArea << std::endl;

    if(in.peek() == '\n'){in.get();} //If the data being deserialized is a base class (ex. the class Route), then at this state the next char is a newline which has to be removed so the next lines are read properly. If an inherited class is being deserialized (ex. the class Door), then the line of data continues

    dbg.close();
}
std::string hax::Route::getType() const{return "Route";}


hax::Door::Door() : Route()
{
    match_key = NULL;
}
hax::Door::Door(std::string name) : Route(name){}
hax::Door::Door(std::string name, Area* from, Area* to) : Route(name, from, to)
{
    match_key = NULL;
}
hax::Door::Door(std::string name, Area* from, Area* to, Key* ke) : Route(name, from, to)
{
    match_key = ke;
}
std::string hax::Door::getType() const{return "door";}
bool hax::Door::isBlocked(Character* const ch) const
{
    if(match_key == NULL){
        return false;
    }
    else if(ch->hasObject(match_key)){
        return false; //has key to unlock door so not blocked
    }else{
        return true;
    }
}
std::string hax::Door::blockMessage() const
{
    if(match_key != NULL){
        return(Route::blockMessage()+" The door requires a "+ match_key->description() +" to open!");
    }else{
        return(Route::blockMessage()+" You should have been able to open this door without any key.");
    }
}
std::string hax::Door::passMessage() const
{
    if(match_key != NULL){
        return(Route::passMessage()+" The door is opened with a "+ match_key->description() +".");
    }else{
        return(Route::passMessage()+" The door does not require any key and is opened without any trouble.");
    }
}
void hax::Door::ToString(std::ostream& out) const
{
    Route::ToString(out);
    out << match_key <<":";
    //NOTE we do not push the Key to serializeQueue because it is not owned by the class Door
}
void hax::Door::FromString(std::istream& in)
{
    Route::FromString(in);
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Door::FromString" << std::endl;

    std::string data;
    std::getline(in,data,':');
    std::string match_keyUID = data;
    match_key = dynamic_cast<Key*>(pointerTable[match_keyUID]);
    dbg << "match_key UID = " << match_keyUID << " | match_key new address = " << match_key << std::endl;

    if(in.peek() == '\n'){in.get();}
    dbg.close();
}


hax::Hatch::Hatch() : Route(){}
hax::Hatch::Hatch(std::string name) : Route(name){}
hax::Hatch::Hatch(std::string name, Area* from, Area* to) : Route(name, from, to){}
bool hax::Hatch::isBlocked(Character* const ch) const
{
    if(ch->totWeight() > 100){
        return false; //trapdoor opens if Character and the Objects he carry weighs enough
    }else{
        return true;
    }
}
std::string hax::Hatch::blockMessage() const
{
    return(Route::blockMessage()+" Get heavier!");
}
std::string hax::Hatch::passMessage() const
{
    return(Route::passMessage()+" The hatch is released by your heavy weight.");
}
std::string hax::Hatch::getType() const{return "hatch";}
