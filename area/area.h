#ifndef AREA_H
#define AREA_H
#include<vector>
#include<queue>
#include<map>
#include "route.h"
#include "../serialize/iserializable.h"
#include "../serialize/simpleHeap.h"
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

    typedef std::map<std::string,Route*> MapRoute;

    //forward declarations
    class Character;
    class Object;
    class Ground;

    class Area : public ISerializable
    {
      public:
        Area();
        Area(std::string);
        Area(const Area&);
        virtual ~Area();

//        Area& operator=(const Area&);
        std::string getName() const;
        SerializableVector<Character*> chars() const;
        Area& neighbor(const Route&) const;
        std::string directions() const;
        virtual std::string description() const;
        void addRoute(Route*);
        virtual void enter(Character*); //TODO return type?
        virtual void leave(Character* const); //TODO return type?
        bool pick_up(Object* const);
        bool drop(Object* const);
        virtual bool rest(Character*);
        virtual Character* spawn();

	/*search functions*/
        Character* getChar(std::string);
        Character* getAnotherRandomCharForInteraction();
        SerializableVector<Character*>::iterator findChar(Character*);
        bool hasChar(Character* const) const;
        Object* getObject(std::string);
        SerializableVector<Object*>::iterator findObject(Object*);
        bool hasObject(Object* const) const;

	/*serialization*/
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);
        virtual std::string getType() const;

	/*data*/
        MapRoute exits; //TODO make this protected
      protected:
        SerializableVector<Character*> vec_char;
//      SerializableVector<Character*> vec_creep; //for characters blocking a road, or other static enemies
        std::string name;
        std::string descr; //description

      private:
        Ground* gnd;
    };
}
#endif
