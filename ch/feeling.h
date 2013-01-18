#ifndef FEELING_H
#define FEELING_H

#define randf() (float)rand()/(float)RAND_MAX

namespace hax
{
    class Character;
    class Area;

    struct Distribution
    {
        double p_go_random;
        double p_fight_random;
        double p_rest;
        double p_talk_to_random;
        void normalize()
        {
            double n = p_go_random+p_fight_random+p_rest+p_talk_to_random;
            p_go_random/=n;
            p_fight_random/=n;
            p_rest=n;
            p_talk_to_random/=n;
        };
        int sample() const
        {
            cumsum randf return int
        }
    };
    class Feeling
    {
      public:
        Feeling(const Character*);
        bool(Character::*)(void) getRandomAction() const;
        virtual Distribution getDistribution() const; 
      private:
        const Character* ch;
	std::vector<bool(Character::*)(void)> actions;
    }
    class HumanFeeling : Feeling
    {
    
    }
    class UndeadFeeling : Feeling
    {

    }
}
