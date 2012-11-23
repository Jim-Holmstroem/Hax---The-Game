#ifndef OBJECT_H
#define OBJECT_H
#include<iostream>
#include<sstream>
#include<cstdlib>
#include "../serialize/iserializable.h"

#define len(a) ( sizeof(a) / sizeof(a[0]) )

namespace hax
{
    class Object : public ISerializable
    {
      public:
        Object();
//        Object(const Object&);
        virtual ~Object(); //is virtual because when adding a Container to a Container and deleting will give segfault

// EXAMPLE
//    Object* ob1 = new Wallet();
//    Container* con1 = new Pocket("con1", 5);
//    Container* con2 = new Pocket("con2", 5);
/*      con1->add(ob1); //OBS adding a Container to a Container, ~Object() must be virtual otherwise delete con will give segfault
        con1->remove(ob1);
        con2->add(ob1);
        delete con1;
        delete con2;
*/
        std::string description() const;
        int getWeight() const;
        int getVolume() const;
        int getPrice() const;

	//serialization
	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);
        virtual std::string getType() const;

      protected:
        /*data*/
        std::string descr;
        int weight; //unit is kg
        int volume; //unit is liter
        int price; //unit is kr
    };

    class Key : public Object
    {
      public:
        Key();
	virtual std::string getType() const;
    };
}
#endif
