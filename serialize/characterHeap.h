#ifndef CHARACTERHEAP_H
#define CHARACTERHEAP_H
#include "../ch/human.h"
#include "../ch/undead.h"

namespace hax {

inline Character* getAllocatedCharacter(std::string charType)
{
    if(charType == "wizard")
    {
        return new Wizard();
    }
    else if(charType == "barbarian")
    {
        return new Barbarian();
    }
};

template<>
inline void SerializableVector<Character*>::ElementFromString(std::istream& in)
{
    std::string charType;
    in >> charType;
    Character* c = getAllocatedCharacter(charType);
    in >> *c;
    content.push_back(c);
};

}
#endif
