#ifndef CONTAINER_H
#define CONTAINER_H
#include<vector>
#include<map>
#include<queue>
#include "object.h"
#include "../serialize/serializablevector.h"

namespace hax
{
    extern std::queue<ISerializable*> serializeQueue;
    extern std::map<std::string,ISerializable*> pointerTable;

    class Container : public Object
    {
      public:
        Container();
//        Container(const Container&);
        ~Container(); //virtual by default since ~Object() is virtual

        Object* operator[](size_t i);

        virtual int hold_weight() const = 0;
        virtual int hold_volume() const = 0;
        bool add(Object* const); //TODO hur används dessa 2 fkner via Character?
        bool remove(Object* const);
        size_t size() const;
        bool empty() const;
	std::string contents() const;
	std::map<std::string, Container*> get_subContainers();
        SerializableVector<hax::Object*>::iterator findObject(Object* const);
        Object* getObject(std::string);
        bool hasObject(Object* const) const;

	//serialization
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);
        virtual std::string getType() const;

      protected:
        /*data*/
        SerializableVector<Object*> vec_obj;
    };

    //used by class Character, picked up items are put here
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

    //used by class Character, contains in-game currency
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

    class Backpack : public Container
    {
      public:
        Backpack();
        virtual int hold_weight() const;
        virtual int hold_volume() const;
	virtual std::string getType() const;
    };

/*    class Pokeball : public Container{ //TODO for the lulz, i.e. not important
      public:
      Pokeball(){
      name = "pokeball";
      }
//      Pokemon poke; //derived from Character
};
*/

}

#endif
