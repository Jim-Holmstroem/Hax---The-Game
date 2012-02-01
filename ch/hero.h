#ifndef HERO_H
#define HERO_H
#include<iostream>
#include "character.h"

namespace hax{
    class Hero : public Character{
      public:
	Hero();
	Hero(std::string); //specify name
	Hero(const Hero&);
	~Hero();

//	Hero& operator=(const Hero&);

	virtual std::string getType() const = 0;
	virtual void action();
/*        virtual void fight(Character*);
        virtual bool pick_up(Object*);
        virtual bool drop(Object*);
        virtual void talk_to(Character*);*/

      protected:
      private:

    };
}
#endif
