#ifndef AREA_H
#define AREA_H
#include<vector>
#include<map>
#include "route.h"
#include "../obj/container.h"
#include "../serialize/serializablevector.h"
#include "../serialize/serializablemap.h"
#include "../serialize/serializablestring.h"
#ifndef DEBUG
#include "../ncurses/logger.h"
#endif

namespace hax{
#ifndef DEBUG
    extern Logger log;
#endif

    typedef std::map<SerializableString, Route*> MapRoute;//TODO SerializableMap

    class Character; //forward declaration
//    class Object;

    class Area : public ISerializable{
      public:
        Area();
        Area(std::string);
        Area(const Area&);
        virtual ~Area();

//        Area& operator=(const Area&);
        std::string getName() const;
        std::vector<Character*> chars() const; //TODO SerializableVector //TODO remove? currently used by level.cpp
        Area& neighbor(const Route&) const;
        std::string directions() const;
        virtual std::string description() const;
        void addRoute(Route*);
        virtual void enter(Character*); //TODO return type?
        virtual void leave(Character* const); //TODO return type?
        bool pick_up(Object* const);
        bool drop(Object* const);
        virtual bool rest(Character*) = 0;
        virtual Character* spawn() = 0;

	//search functions
        Character* getChar(std::string);
        std::vector<Character*>::iterator findChar(Character*); //TODO SerializableVector
        bool hasChar(Character* const) const;
        Object* getObject(std::string);
        std::vector<Object*>::iterator findObject(Object*); //TODO SerializableVector
        bool hasObject(Object* const) const;

	//serialization
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);

	//data objects
        MapRoute exits; //TODO make this protected
      protected:
        std::vector<Character*> vec_char; //TODO SerializableVector
//      SerializableVector<Character*> vec_creep; //for characters blocking a road, or other static enemies
        std::string name;
        std::string descr; //description

        //definition of Ground put in protected because class Shop needs it
        class Ground : public Container{ //like a big reservoir
          public:
            Ground();
            Ground(std::string);
            virtual int hold_weight() const;
            virtual int hold_volume() const;
	    virtual std::string getType() const;
        };

      private:
        Ground* gnd;
    };
}
#endif
