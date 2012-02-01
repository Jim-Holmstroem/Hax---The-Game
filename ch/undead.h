#ifndef UNDEAD_H
#define UNDEAD_H
#include "character.h"

namespace hax{
    class Undead : public Character{
      public:
        Undead();
        Undead(std::string);
        ~Undead();

	virtual void action();
        virtual std::string getType() const;
        virtual void attack(Character*);
	virtual void talk_to(Character*); //TODO
      private:
        void regenerate();

    };
    class Skeleton : public Undead{
      public:
	Skeleton();
	Skeleton(std::string);
	std::string getType() const;
//no definition of attack here so will automatically use Undead::attack(ch)

      private:
        void initStats();
    };

    class Necromancer : public Undead{
      public:
	Necromancer();
	Necromancer(std::string);
	std::string getType() const;
	void attack(Character*);

      private:
	void initStats();
    };

}
#endif
