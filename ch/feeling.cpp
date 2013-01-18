#include<cstdlib>
#include "feeling.h"
#include "character.h"

hax::Feeling::Feeling(const hax::Distrbution dist)
{
    this->dist = dist;
    dist.normalize();
    actions.push_back(&Character::go_random);
    actions.push_back(&Character::fight_random);
    actions.push_back(&Character::rest);
    actions.push_back(&Character::talk_to_random);
}

bool(hax::Character::*)(void) hax::Feeling::getRandomAction() const
{
    return actions[dist.sample()];
}
