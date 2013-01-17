#include<cstdio>
#include<cstdlib>
#include<fstream>
#include "human.h"

hax::Human::Human() : Character(){}
hax::Human::Human(std::string name, bool control) : Character(name, control){}
hax::Human::Human(const Human& hu) : Character(hu){}
hax::Human::~Human(){}

/*hax::Human& hax::Human::operator=(const Human& hu){
  return *this;
  }*/

void hax::Human::view_stats() const
{
    Character::view_stats();
}
void hax::Human::attack(Character* ch)
{
    int damage = strength;
    ch->curHp -= damage;
    std::cout << this->getName() << " hits " << ch->getName() << " for " << damage << " damage" << std::endl;
}
void hax::Human::talk_to(Character* ch)
{
} //TODO
void hax::Human::ToString(std::ostream& out) const
{
    Character::ToString(out);
}
void hax::Human::FromString(std::istream& in)
{
    Character::FromString(in);
    if(in.peek() == '\n'){in.get();}
}
std::string hax::Human::getType() const{return "human";}
void hax::Human::initStats(int curHp, int maxHp, int strength, int weight)
{
    Character::initStats(curHp, maxHp, strength, weight);
}


hax::Wizard::Wizard() : Human()
{
    initStats(30,30,5,60);
}
hax::Wizard::Wizard(std::string name, bool control) : Human(name, control)
{
    initStats(30,30,5,60);
}
hax::Wizard::Wizard(const Wizard& wi) : Human(wi)
{
    curMp = wi.curMp;
    maxMp = wi.maxMp;
}
void hax::Wizard::view_stats() const
{
    Human::view_stats();
#ifdef DEBUG
    std::cout <<"MP: "<< curMp <<"/"<< maxMp << std::endl;
#else
    std::ostringstream oss;
    oss <<"MP: "<< curMp <<"/"<< maxMp;
    hax::log.write(oss.str());
#endif
}
void hax::Wizard::attack(Character* ch)
{
    if(curMp >= 5){
        std::cout << getName() << " casts Lightning Bolt for 10 dmg" << std::endl;
        curMp -= 5;
        ch->curHp -= 10;
    }else{
        Human::attack(ch);
    }
}
void hax::Wizard::talk_to(Character* ch)
{
} //TODO
void hax::Wizard::initStats(int curHp, int maxHp, int strength, int weight)
{
    Human::initStats(curHp, maxHp, strength, weight);
    curMp = 100;
    maxMp = curMp;
}
void hax::Wizard::ToString(std::ostream& out) const
{
    Human::ToString(out);
    out << curMp <<":"<< maxMp <<":";
}
void hax::Wizard::FromString(std::istream& in)
{
    Human::FromString(in);
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Wizard::FromString" << std::endl;

    std::string data;
    std::getline(in,data,':');
    curMp = std::atoi(data.c_str());
    dbg << "Current MP = " << curMp << std::endl;

    std::getline(in,data,':');
    maxMp = std::atoi(data.c_str());
    dbg << "Max MP = " << maxMp << std::endl;

    if(in.peek() == '\n'){in.get();}
    dbg.close();
}
std::string hax::Wizard::getType() const{return "wizard";}


hax::Barbarian::Barbarian() : Human()
{
    initStats(50,50,10,80);
}
hax::Barbarian::Barbarian(std::string name, bool control) : Human(name, control)
{
    initStats(50,50,10,80);
}
hax::Barbarian::Barbarian(const Barbarian& ba) : Human(ba){}
void hax::Barbarian::attack(Character* ch)
{
    Human::attack(ch);
    std::cout << this->getName() << " attacks again!" << std::endl;
    Human::attack(ch);
}
void hax::Barbarian::talk_to(Character* ch)
{
} //TODO
std::string hax::Barbarian::getType() const{return "barbarian";}
