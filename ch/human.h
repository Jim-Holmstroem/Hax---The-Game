#ifndef HUMAN_H
#define HUMAN_H
#include "character.h"
#include<iostream>

namespace hax
{
    class Undead;
    class Human : public Character
    {
      public:
        Human();
        Human(std::string, bool);
        Human(const Human&);
        ~Human();

//        Human& operator=(const Human&);
        virtual void view_stats() const;
        virtual void attack(Character*);
        virtual void talk_to(Character*);

        virtual void ToString(std::ostream&) const;
        virtual void FromString(std::istream&);
        virtual std::string getType() const;
        virtual Feeling getFeeling() const;

      protected:
        virtual void initStats(int,int,int,int);
    };

    class Wizard : public Human
    {
      public:
        Wizard();
        Wizard(std::string, bool);
        Wizard(const Wizard&);
        virtual void view_stats() const;
        virtual void attack(Character*);

        virtual void ToString(std::ostream&) const;
        virtual void FromString(std::istream&);
        virtual std::string getType() const;

      protected:
        int curMp; //magic points
        int maxMp;
        virtual void initStats(int,int,int,int);
    };

    class Barbarian : public Human
    {
      public:
	Barbarian();
	Barbarian(std::string, bool);
	Barbarian(const Barbarian&);
	virtual void attack(Character*);

	virtual std::string getType() const;
    };
}
#endif
