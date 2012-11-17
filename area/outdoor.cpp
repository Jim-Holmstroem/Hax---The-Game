#include "outdoor.h"
#include "../ch/human.h"

hax::Outdoor::Outdoor() : Area(){}
hax::Outdoor::Outdoor(std::string name) : Area(name){}
hax::Outdoor::Outdoor(const Outdoor& ou) : Area(ou){}
hax::Outdoor::~Outdoor(){}
void hax::Outdoor::enter(Character* ch)
{
    Area::enter(ch);
}
void hax::Outdoor::leave(Character* ch)
{
    Area::leave(ch);
}
bool hax::Outdoor::rest(Character* ch)
{
    Area::rest(ch);
    int maxHp = ch->getmaxHp();
    int heal = maxHp/4;
    ch->curHp += heal;
    std::cout << ch->getName() << " healed " << heal << " HP" << std::endl;
    if(ch->curHp > maxHp){
        ch->curHp = maxHp;
    }
    return true;
}


hax::Forest::Forest() : Outdoor(){}
hax::Forest::Forest(std::string name) : Outdoor(name)
{
    descr = "You are surrounded by trees.";
}
std::string hax::Forest::getType() const{return "forest";}
bool hax::Forest::rest(Character* ch)
{
    Outdoor::rest(ch);
/*    if(vec_char.size() > 1){ //other Chars here beside yourself
      vec_char[0]->fight(ch); //TODO slumpa chans och index
      }
*/
    std::cout <<"You are attacked by a robber when sleeping!"<< std::endl;
    Character* robber = new Barbarian("Robber"); //TODO slumpa chans att bli anfallen
    enter(robber);
    robber->fight(ch);
    return true;
}
hax::Character* hax::Forest::spawn()
{
//list of random names
    Character* creep = new Barbarian("spawnedBarb");
    enter(creep);
    return(creep);
}
