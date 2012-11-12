#ifndef ROUTEHEAP_H
#define ROUTEHEAP_H
#include "../area/road.h"

namespace hax {

inline Route* getAllocatedRoute(std::string routeType)
{
    if(routeType == "road")
    {
        return new Road();
    }
};

template<>
inline void SerializableMap<SerializableString,Route*>::ElementFromString(std::istream& in)
{
    std::string str;
    in >> str;
    SerializableString keyData;
    in >> keyData;
    std::pair<SerializableString,Route*> elem(keyData, getAllocatedRoute(str));
    in >> *(elem.second);
    content.insert(elem);
};

}
#endif
