#include<cstdlib>
#include "feeling.h"
#include "character.h"



hax::Feeling::Feeling(const Character* ch)
{
    this->ch = ch;
    randomActions.push_back(&Character::go_random);
    randomActions.push_back(&Character::fight_random);
    randomActions.push_back(&Character::rest);
    randomActions.push_back(&Character::talk_to_random);
}

Distribution hax::Feeling::getDistribution() const
{
    Distribution dist = {
        /*p_go_random=*/ , f1(ch) //could easily depend on indoor/outdoor
        /*p_fight_random=*/ , ..
        /*p_rest=*/ , ..
        /*p_talk_to_random=*/ , ..
    };
    dist.normalize();
    return dist;
}

Distribution hax::HumanFeeling::getDistribution() const
{

Distribution hax::UndeadFeeling::getDistribution() const
{

bool(Character::*)(void) getRandomAction() const
{
    return actions[getDistribution().sample()];
}
