#include<cstdio>
#include<cstdlib>
#include<fstream>
#include "road.h"
#include "../ch/character.h"
#include "../area/area.h"
#include "../obj/container.h" //needed for Ground
#include "../obj/obstacle.h"

hax::Road::Road() : Route()
{
    enemy = NULL;
    p_obs = NULL;
}
hax::Road::Road(std::string name) : Route(name)
{
    enemy = NULL;
    p_obs = NULL;
}
hax::Road::Road(std::string name, Area* from, Area* to) : Route(name, from, to)
{
    enemy = NULL;
    p_obs = NULL;
}
hax::Road::Road(std::string name, Area* from, Area* to, Character* ch) : Route(name, from, to) //TODO change to Foe*
{
    enemy = ch;
    p_obs = NULL;
    thisArea->enter(ch); //this Character is not added to global map so it will not perform random actions
}
hax::Road::Road(std::string name, Area* from, Area* to, Obstacle* ob) : Route(name, from, to)
{
    enemy = NULL;
    if(thisArea->pick_up(ob)){ //in case Area cannot pick up
        p_obs = ob;
    }else{
        p_obs = NULL;
//      delete ob;
    }
}
bool hax::Road::isBlocked(Character* const ch) const
{
/*    if(enemy == 0 && p_obs == 0){ //this check doesn't seem to know if object is deleted
      return false;
      }
*/
    if(thisArea->hasChar(enemy)){
        return true; //a Char is blocking the road
    }else if(thisArea->hasObject(p_obs)){
        return true; //TODO check for Obstacle
    }else{
        return false;
    }
}
std::string hax::Road::blockMessage() const
{
    if(thisArea->hasChar(enemy)){
        return(enemy->getName() +" is standing in the way!");
    }else if(thisArea->hasObject(p_obs)){
        return("There is a "+ p_obs->description() +" blocking the way!");
    }else{
	return("Why am I blocked? DEBUG");
    }
}
std::string hax::Road::passMessage() const
{
    return("The road is clear.");
}
void hax::Road::ToString(std::ostream& out) const
{
    Route::ToString(out);
    out << enemy <<":"<< p_obs <<":";
    //NOTE we do not push the pointers to serializeQueue because they are not owned by the class Road
}
void hax::Road::FromString(std::istream& in)
{
    Route::FromString(in);
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Road::FromString" << std::endl;

    std::string data;
    std::getline(in,data,':');
    std::string enemyUID = data;
    enemy = dynamic_cast<Character*>(pointerTable[enemyUID]);
    dbg << "enemy UID = " << enemyUID << " | enemy new address = " << enemy << std::endl;

    std::getline(in,data,':');
    std::string p_obsUID = data;
    p_obs = dynamic_cast<Obstacle*>(pointerTable[p_obsUID]);
    dbg << "p_obs UID = " << p_obsUID << " | p_obs new address = " << p_obs << std::endl;

    if(in.peek() == '\n'){in.get();}
    dbg.close();
}
std::string hax::Road::getType() const{return "road";}
