#include "obstacle.h"

hax::Obstacle::Obstacle()
{
    weight = -1; //TODO
    volume = 0;
    price = 0;
}
hax::Obstacle::Obstacle(void* vo)
{
//    criteria = vo;
}
hax::Obstacle::~Obstacle()
{
    std::cout << "~Obstacle()" << std::endl;
}

hax::Tree::Tree()
{
    const char* attr[] = { //attributes
        "oak",
        "willow",
        "dead",
    };
    descr = attr[rand() % len(attr)];
}
std::string hax::Tree::getType() const{return "tree";}
