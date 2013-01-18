#include "undead.h"
#include "feeling.h"
#include "../area/area.h"

hax::Undead::Undead() : Character(){
}
hax::Undead::Undead(std::string name, bool control) : Character(name, control){
}
hax::Undead::~Undead(){
}
void hax::Undead::talk_to(Character* ch)
{
    const std::string sounds[] = {
        "THRRPTT! ",
        "BLEH HEH HEH HEE ",
        "GURGLE BLEPT THIPT THPT HICK GRTSTHPT",
        "JUSTIN BIEBER?",
        "crunch..munch..crunch..",
    };
    std::cout << getName() << ":" << sounds[rand()%len(sounds)] << std::endl;

}
void hax::Undead::attack(Character* ch)
{
    int damage = ceil(5 + strength*(1-(float)curHp/maxHp));
    ch->curHp -= damage;
    std::cout << this->getName() << " hits " << ch->getName() << " for " << damage << " damage" << std::endl;
    regenerate();
}
std::string hax::Undead::getType() const{return "undead";}
void hax::Undead::initStats(int curHp, int maxHp, int strength, int weight)
{
    Character::initStats(curHp, maxHp, strength, weight);
}
void hax::Undead::regenerate()
{
    int regen = maxHp/10;
    curHp += regen;
    if(curHp > maxHp){
        curHp = maxHp;
    }
    std::cout << getName() << " regenerated " << regen << " HP" << std::endl;
}


hax::Skeleton::Skeleton() : Undead()
{
    initStats(30,30,2,5);
}
hax::Skeleton::Skeleton(std::string name, bool control) : Undead(name, control)
{
    initStats(30,30,2,5);
}
std::string hax::Skeleton::getType() const{return "skeleton";}


hax::Necromancer::Necromancer() : Undead()
{
    initStats(50,50,7,40);
}
hax::Necromancer::Necromancer(std::string name, bool control) : Undead(name, control)
{
    initStats(50,50,7,40);
}
void hax::Necromancer::attack(Character* ch) //TODO
{
    Undead::attack(ch);
    Skeleton* minion = new Skeleton("Skelly",0);
    std::cout << getName() << " raised a " << minion->getType() << " from the dead!" << std::endl;
    curArea->enter(minion);
    minion->attack(ch);
    delete minion;
}
std::string hax::Necromancer::getType() const{return "necromancer";}
void hax::Necromancer::initStats(int curHp, int maxHp, int strength, int weight)
{
    Undead::initStats(curHp,maxHp,strength,weight);
//    curMp = 80;
//    maxMp = curMp;
}
