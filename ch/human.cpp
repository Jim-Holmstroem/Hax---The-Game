#include "human.h"

hax::Human::Human(){
}
hax::Human::Human(std::string name) : Hero(name){
}
hax::Human::Human(const Human& hu) : Hero(hu){
}
hax::Human::~Human(){
}

/*hax::Human& hax::Human::operator=(const Human& hu){
  return *this;
  }*/

std::string hax::Human::getType() const{
    return "human";
}
void hax::Human::attack(Character* ch){
    int damage = strength;
    ch->curHp -= damage;
    std::cout << this->getName() << " hits " << ch->getName() << " for " << damage << " damage" << std::endl;
}
void hax::Human::talk_to(Character* ch){
} //TODO
void hax::Human::ToString(std::ostream& out) const{
    Character::ToString(out);
    out << getType();
}; //TODO
void hax::Human::FromString(std::istream& in){
}; //TODO

hax::Wizard::Wizard(){
    initStats();
}
hax::Wizard::Wizard(std::string name) : Human(name){
    initStats();
}
hax::Wizard::Wizard(const Wizard& wi) : Human(wi){
    curMp = wi.curMp;
    maxMp = wi.maxMp;
}
std::string hax::Wizard::getType() const{
    return (Human::getType() +" wizard");
}
void hax::Wizard::view_stats() const{
    Character::view_stats();
#ifdef DEBUG
    std::cout <<"MP: "<< curMp <<"/"<< maxMp << std::endl;
#else
    std::ostringstream oss;
    oss <<"MP: "<< curMp <<"/"<< maxMp;
    hax::log.write(oss.str());
#endif
}
void hax::Wizard::attack(Character* ch){
    if(curMp >= 5){
        std::cout << getName() << " casts Lightning Bolt for 10 dmg" << std::endl;
        curMp -= 5;
        ch->curHp -= 10;
    }else{
        Human::attack(ch);
    }
}
void hax::Wizard::talk_to(Character* ch){
} //TODO
void hax::Wizard::initStats(){
    curHp = 30;
    maxHp = curHp;
    curMp = 100;
    maxMp = curMp;
    strength = 5;
    weight = 60;
}
void hax::Wizard::ToString(std::ostream& out) const{
}; //TODO
void hax::Wizard::FromString(std::istream& in){
}; //TODO

hax::Barbarian::Barbarian(){
    initStats();
}
hax::Barbarian::Barbarian(std::string name) : Human(name){
    initStats();
}
hax::Barbarian::Barbarian(const Barbarian& ba) : Human(ba){
}
std::string hax::Barbarian::getType() const{
    return (Human::getType() +" barbarian");
}
void hax::Barbarian::attack(Character* ch){
    Human::attack(ch);
    std::cout << this->getName() << " attacks again!" << std::endl;
    Human::attack(ch);
}
void hax::Barbarian::talk_to(Character* ch){
} //TODO
void hax::Barbarian::initStats(){
    curHp = 50;
    maxHp = curHp;
    strength = 10;
    weight = 80;
}
