#ifndef CHARACTER_H
#define CHARACTER_H
#include<iostream>
#include<sstream>
#include<vector>
#include<map>
#include<stack>
#include<queue>
#include<cmath>
#include<string>
#include "../serialize/iserializable.h"
#ifndef DEBUG
#include "../ncurses/logger.h"
#endif

#define len(a) ( sizeof(a) / sizeof(a[0]) )

namespace hax
{
#ifndef DEBUG
    extern Logger log;
#endif
    extern std::queue<ISerializable*> serializeQueue;
    extern std::map<std::string,ISerializable*> pointerTable;

    /*forward declarations*/
    class Area;
    class Object;
    class Route;
    class Container;
    class Pocket;
    class Wallet;
//    template <typename T> class Container;

    class Character : public ISerializable
    {
      public:
        Character();
        Character(std::string, bool);
        Character(const Character&);
        virtual ~Character();

//        Character& operator=(const Character&);
        bool isControllable() const;
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
        void give(Object* const);

	/*serialization*/
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);
        virtual std::string getType() const;

	/*public data*/
        Area* curArea;
        int curHp; //if health<0 then sleeping with the fishes TODO this should be protected, but then ch->curHp is protected in derived classes

      protected:
        bool controllable;
        int maxHp;
        int strength;
        int weight; //unit is kg
        virtual void initStats(int,int,int,int);

      private:
	/*private data*/
        std::string name;
        Pocket* inventory;
        Wallet* myWallet;
	Container* curContainer; //command "cd" and "ls" uses this
	std::stack<Container*> folderStack;
    };
}
#endif
