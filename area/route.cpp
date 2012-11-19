#include "route.h"
#include "../ch/character.h"
#include "../area/area.h"
//#include "../obj/container.h" //needed for inventory

hax::Route::Route()
{
    thisArea = NULL;
    nextArea = NULL;
}
hax::Route::Route(std::string n, Area* from, Area* to)
{
    name = n;
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
    out << this <<":"<< getType() <<":"<< name <<":"<< view <<":"<< thisArea <<":"<< nextArea << std::endl;
}
void hax::Route::FromString(std::istream& in)
{
} //TODO
std::string hax::Route::getType() const
{
    return "Route";
}

hax::Door::Door() : Route(){
    match_key = NULL;
}
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

hax::Hatch::Hatch() : Route(){}
hax::Hatch::Hatch(std::string name, Area* from, Area* to) : Route(name, from, to){
//    type = "hatch";
}
std::string hax::Hatch::getType() const{return "hatch";}
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
