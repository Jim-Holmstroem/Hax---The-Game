#include "obstacle.h"

hax::Obstacle::Obstacle()
{
    weight = -1; //weight is negative so that the Obstacle cannot be picked up
    volume = 0;
    price = 0;
}
hax::Obstacle::~Obstacle()
{
    std::cout << "~Obstacle()" << std::endl;
}


hax::Tree::Tree() : Obstacle()
{
    const char* attr[] = { //attributes
        "oak",
        "willow",
        "dead",
    };
    descr = attr[rand() % len(attr)];
}
std::string hax::Tree::getType() const{return "tree";}
