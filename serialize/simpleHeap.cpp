#include "simpleHeap.h"
#include "iserializable.h"
#include "../world/level.h"
#include "../ch/human.h"
#include "../ch/undead.h"
#include "../area/indoor.h"
#include "../area/outdoor.h"
#include "../area/road.h"
#include "../obj/container.h"
#include "../obj/obstacle.h"
#include "../obj/coin.h"

hax::ISerializable* hax::allocateData(std::string type, std::string name)
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
    else if(type == "road"){return new Road(name);}
    else if(type == "door"){return new Door(name);}
    else if(type == "hatch"){return new Hatch(name);}
    else if(type == "pocket"){return new Pocket();}
    else if(type == "wallet"){return new Wallet();}
    else if(type == "backpack"){return new Backpack();}
    else if(type == "key"){return new Key();}
    else if(type == "coin"){return new Coin();}
    else{return NULL;}
}
