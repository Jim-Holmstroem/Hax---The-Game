#ifndef HUMAN_H
#define HUMAN_H
#include "hero.h"
#include<iostream>

namespace hax{
    class Human : public Hero{
      public:
        Human();
        Human(std::string);
        Human(const Human&);
        ~Human();

//        Human& operator=(const Human&);
        virtual std::string getType() const;
        virtual void attack(Character*);
        virtual void talk_to(Character*);
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);
      protected:
      private:
    };

    class Wizard : public Human{
      public:
        Wizard();
        Wizard(std::string);
	Wizard(const Wizard&);
	virtual std::string getType() const;
	virtual void view_stats() const;
	virtual void attack(Character*);
        virtual void talk_to(Character*);
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);

      protected:
        int curMp; //magic points
        int maxMp;
      private:
        virtual void initStats();

    };

    class Barbarian : public Human{
      public:
	Barbarian();
	Barbarian(std::string);
	Barbarian(const Barbarian&);
	virtual std::string getType() const;
	virtual void attack(Character*);
        virtual void talk_to(Character*);
      private:
	virtual void initStats();

    };
}
#endif
