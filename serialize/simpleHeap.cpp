#include "simpleHeap.h"
#include "../world/level.h"
#include "../ch/human.h"
#include "../ch/undead.h"
#include "../area/indoor.h"
#include "../area/outdoor.h"
#include "../obj/container.h"
#include "../obj/obstacle.h"
#include "../area/road.h"

hax::ISerializable* hax::allocateData(std::string type)
{
    if(type == "level"){return new Level();}
    else if(type == "wizard"){return new Wizard();}
    else if(type == "barbarian"){return new Barbarian();}
    else if(type == "skeleton"){return new Skeleton();}
    else if(type == "necromancer"){return new Necromancer();}
    else if(type == "forest"){return new Forest();}
    else if(type == "castle"){return new Castle();}
    else if(type == "school"){return new School();}
    else if(type == "shop"){return new Shop();}
    else if(type == "backpack"){return new Backpack();}
    else if(type == "key"){return new Key();}
    else if(type == "road"){return new Road();}
    else if(type == "door"){return new Door();}
    else{return NULL;}
}
hax::Character* hax::getAllocatedCharacter(std::string type)
{
    if(type == "wizard"){return new Wizard();}
    else if(type == "barbarian"){return new Barbarian();}
    else if(type == "skeleton"){return new Skeleton();}
    else if(type == "necromancer"){return new Necromancer();}
    else{return NULL;}
}
hax::Area* hax::getAllocatedArea(std::string type)
{
    if(type == "forest"){return new Forest();}
    else if(type == "castle"){return new Castle();}
    else if(type == "school"){return new School();}
    else if(type == "shop"){return new Shop();}
    else{return NULL;}
}
hax::Object* hax::getAllocatedObject(std::string type)
{
    if(type == "backpack"){return new Backpack();}
    else if(type == "key"){return new Key();}
    else{return NULL;}
}
hax::Route* hax::getAllocatedRoute(std::string type)
{
    if(type == "road"){return new Road();}
    else if(type == "door"){return new Door();}
    else{return NULL;}
}
