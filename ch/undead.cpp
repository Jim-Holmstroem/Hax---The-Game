#include "undead.h"
#include "../area/area.h"

hax::Undead::Undead(){
}
hax::Undead::Undead(std::string name) : Character(name){
}
hax::Undead::~Undead(){
}
void hax::Undead::action(){
}
std::string hax::Undead::getType() const{
    return "undead";
}
void hax::Undead::talk_to(Character* ch){
}
void hax::Undead::attack(Character* ch){
    int damage = ceil(5 + strength*(1-(float)curHp/maxHp));
    ch->curHp -= damage;
    std::cout << this->getName() << " hits " << ch->getName() << " for " << damage << " damage" << std::endl;
    regenerate();
}
void hax::Undead::regenerate(){
    int regen = maxHp/10;
    curHp += regen;
    if(curHp > maxHp){
        curHp = maxHp;
    }
    std::cout << getName() << " regenerated " << regen << " HP" << std::endl;
}

hax::Skeleton::Skeleton(){
    initStats();
}
hax::Skeleton::Skeleton(std::string name) : Undead(name){
    initStats();
}
std::string hax::Skeleton::getType() const{
    return (Undead::getType() +" skeleton");
}
void hax::Skeleton::initStats(){
    curHp = 30;
    maxHp = curHp;
    strength = 2;
    weight = 5;
}

hax::Necromancer::Necromancer(){
    initStats();
}
hax::Necromancer::Necromancer(std::string name) : Undead(name){
    initStats();
}
std::string hax::Necromancer::getType() const{
    return (Undead::getType() +" necromancer");
}
void hax::Necromancer::attack(Character* ch){ //TODO
    Undead::attack(ch);
    Skeleton* minion = new Skeleton();
    std::cout << getName() << " raised a " << minion->getType() << " from the dead!" << std::endl;
    curArea->enter(minion);
    minion->attack(ch);
    delete minion;
}
void hax::Necromancer::initStats(){
    curHp = 50;
    maxHp = curHp;
//          curMp = 80;
//          maxMp = curMp;
    strength = 7;
    weight = 40;
}
