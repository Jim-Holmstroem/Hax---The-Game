#include<cstdlib>
#include "feeling.h"
#include "character.h"

typedef bool(hax::Character::*ActionPtr)(void);

hax::Feeling::Feeling(const hax::Distribution dist)
{
    this->dist = dist;
    this->dist.normalize();
    actions.push_back(&Character::go_random);
    actions.push_back(&Character::fight_random);
    actions.push_back(&Character::rest);
    actions.push_back(&Character::talk_to_random);
}

hax::ActionPtr hax::Feeling::getRandomAction() const
{
    return actions[dist.sample()];
}
