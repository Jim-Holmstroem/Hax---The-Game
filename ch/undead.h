#ifndef UNDEAD_H
#define UNDEAD_H
#include "character.h"

namespace hax
{
    class UndeadFeeling;
    class Undead : public Character
    {
      public:
        Undead();
        Undead(std::string, bool);
        ~Undead();
        virtual std::string getType() const;
        virtual void attack(Character*);
	virtual void talk_to(Character*);

      protected:
        virtual void initStats(int,int,int,int);

      private:
        void regenerate();
    };

    class Skeleton : public Undead
    {
      public:
	Skeleton();
	Skeleton(std::string, bool);
	std::string getType() const;
        //no definition of attack here so will automatically use Undead::attack(ch)
    };

    class Necromancer : public Undead
    {
      public:
	Necromancer();
	Necromancer(std::string, bool);
	void attack(Character*);
	std::string getType() const;

      protected:
	virtual void initStats(int,int,int,int);
    };

}
#endif
