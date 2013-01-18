#include<cstdlib>
#include "feeling.h"
#include "character.h"



hax::Feeling::Feeling(const Distrbution<4> dist)
{
    this->dist = dist;
    dist.normalize();
    randomActions.push_back(&Character::go_random);
    randomActions.push_back(&Character::fight_random);
    randomActions.push_back(&Character::rest);
    randomActions.push_back(&Character::talk_to_random);
}

bool(Character::*)(void) getRandomAction() const
{
    return actions[dist.sample()];
}
