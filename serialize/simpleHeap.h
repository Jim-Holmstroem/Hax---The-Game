#ifndef SIMPLEHEAP_H
#define SIMPLEHEAP_H
#include "serializablevector.h"
#include "serializablemap.h"
#include "serializablestring.h"

namespace hax
{
    /*forward declarations*/
    class Character;
    class Area;
    class Object;
    class Route;

    ISerializable* allocateData(std::string);
    Character* getAllocatedCharacter(std::string);
    Area* getAllocatedArea(std::string);
    Object* getAllocatedObject(std::string);
    Route* getAllocatedRoute(std::string);
}
#endif
