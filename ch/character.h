#ifndef CHARACTER_H
#define CHARACTER_H
#include<iostream>
#include<sstream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<map>
#include<stack>
#include<queue>
#include<cmath>
#include "../serialize/iserializable.h"
#include "../obj/container.h"
#ifndef DEBUG
#include "../ncurses/logger.h"
#endif

namespace hax
{
#ifndef DEBUG
    extern Logger log;
#endif

    /*forward declarations*/
    class Area;
    class Object;
    class Route;
//    template <typename T> class Container;

    class Character : public ISerializable
    {
      public:
        Character();
        Character(std::string);
        Character(const Character&);
        virtual ~Character();

//        Character& operator=(const Character&);
        bool alignment() const;
        std::string getName() const;
        int getcurHp() const;
        int getmaxHp() const;
        int getStr() const;
        int totWeight() const;
        bool hasObject(Object* const) const;

        virtual void action(); //TODO return type? depends on current situation, for Hero let it be input from getch, for Foe let it be random
        virtual void view_stats() const;
        void view_area() const;
	void view_inventory() const;
	void view_curContainer() const;
	bool change_curContainer(std::string);
//	bool move_obj(std::string, std::string); //arg is objName, destination
	bool move_obj(std::string, std::string, std::string); //arg is objName, source, destination
        bool go(std::string);
        bool go(const Route&);
        bool go_random();
        bool fight(std::string);
        void fight(Character*);
        bool fight_random();
        virtual void attack(Character*);
        virtual void talk_to(Character*); //TODO return type?
        bool pick_up(std::string);
        bool drop(std::string);
        bool buy(std::string);
        bool sell(std::string);
        bool rest();

	/*serialization*/
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);
        virtual std::string getType() const;

	/*public data*/
        Area* curArea;
        int curHp; //if health<0 then sleeping with the fishes TODO this should be protected, but then ch->curHp is protected in derived classes

      protected:
        bool align; //use dynamic_cast instead? move to hero/foe? pokemon convert
        int maxHp;
        int strength;
        int weight; //unit is kg
        virtual void initStats(int,int,int,int);

      private:
        //supposed to contain in-game currency
        class Wallet : public Container
        {
          public:
            Wallet();
            Wallet& operator+=(const int);
            Wallet& operator-=(const int);
            virtual int hold_weight() const;
            virtual int hold_volume() const;
	    virtual std::string getType() const;
        };

//OBS! Both Pocket and Ground are private in character.h resp area.h so they cannot be added to another container from outside
        class Pocket : public Container
        {
          public:
            Pocket();
            Pocket(std::string, unsigned int);
            virtual int hold_weight() const;
            virtual int hold_volume() const;
	    virtual std::string getType() const;
          private:
            unsigned int maxSize;
        };

	/*private data*/
        std::string name;
        Pocket* inventory;
        Wallet myWallet;
	Container* curContainer; //command "cd" and "ls" uses this
	std::stack<Container*> folderStack;
    };
}
#endif
