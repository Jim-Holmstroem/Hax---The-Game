#ifndef AREAHEAP_H
#define AREAHEAP_H
#include "../area/indoor.h"
#include "../area/outdoor.h"

namespace hax {

inline Area* getAllocatedArea(std::string areaType)
{
    if(areaType == "forest")
    {
        return new Forest("yeah");
    }
    else if(areaType == "castle")
    {
        return new Castle("yeah");
    }
};

template<>
inline void SerializableVector<Area*>::ElementFromString(std::istream& in)
{
    std::string areaType;
    in >> areaType;
    Area* a = getAllocatedArea(areaType);
    in >> *a;
    content.push_back(a);
};

}
#endif
