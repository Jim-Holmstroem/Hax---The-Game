#include "hero.h"

hax::Hero::Hero(){
    align = true;
};
hax::Hero::Hero(std::string name) : Character(name){
};
hax::Hero::Hero(const Hero& he) : Character(he){
};
hax::Hero::~Hero(){
};

/*hax::Hero& hax::Hero::operator=(const Hero& he){
    return *this;
};*/

void hax::Hero::action(){ //TODO
};
/*void hax::Hero::fight(Character* ch){
};
void hax::Hero::talk_to(Character* ch){
};*/
