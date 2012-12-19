#ifndef LEVEL_H
#define LEVEL_H
#include<iostream>
#include<vector>
#include<map>
#include<queue>
#include "operation.h"
#include "../serialize/serializablevector.h"
#ifndef DEBUG
#include "../ncurses/logger.h"
#endif

namespace hax
{
#ifndef DEBUG
    extern Logger log;
#endif
    extern std::queue<ISerializable*> serializeQueue;
    extern std::map<std::string,ISerializable*> pointerTable;

    /*forward declarations*/
    class Area;
    class Character;
    class Container;
    class OperationVoid;

    class Level : public ISerializable
    {
      public:
        Level();
        Level(int); //specify number of areas
//        Level(const Level&);
        ~Level();

        void add(Area*);
        void add(Character*);
	void parse(std::vector<std::string>);

	//serialization
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);
	virtual std::string getType() const;

      protected:
      private:
	void updatePlayers();
	void action();
	bool focus(std::string);
        void info();
	void help();
	void spawn();
	bool kill(std::string);
        bool save(std::string);
        bool load(std::string);
        void deleteAreas();

        /*data*/
        SerializableVector<Area*> vec_area;

        /*used for operations*/
	std::map<std::string, Operation*> opmap;
	std::vector<bool(Character::*)(void)> randomActions;
	std::map<std::string, std::string> levelHelp;
	std::map<std::string, std::string> charHelp;
        std::map<std::string, Character*> players; //global map with controlled characters, iterate over this and use action(), string is Character name
	Character* curChar;
	std::string curCharName;
    };
}
#endif
