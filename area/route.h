#ifndef ROUTE_H
#define ROUTE_H
#include<iostream>
#include "../serialize/iserializable.h"

namespace hax{
    class Character;
    class Area; //using forward declaration!!!! otherwise circular inclusion, this is possible when we only have pointers
//http://www.codeguru.com/forum/showthread.php?t=383253
    class Container;
    class Key;

    class Route : public ISerializable{
      public:
        Route(std::string, Area*, Area*);
        ~Route();

        std::string getName() const;
        virtual bool isBlocked(Character* const) const = 0;
        virtual std::string blockMessage() const = 0;
	virtual std::string passMessage() const = 0;

	virtual void ToString(std::ostream&) const;
	virtual void FromString(std::istream&);

        Area* thisArea;
        Area* nextArea; //could make interesting maps by changing this during game

      protected:
//	std::string type;
        std::string view; //what the character sees when looking in this direction TODO

      private:
        std::string name; //search for this string when parsing
    };

    class Door : public Route{
      public:
	Door(std::string, Area*, Area*);
	Door(std::string, Area*, Area*, Key*);
	virtual std::string getType() const;
	virtual bool isBlocked(Character*) const;
	virtual std::string blockMessage() const;
	virtual std::string passMessage() const;
      private:
        Key* match_key;
    };

    class Hatch : public Route{ //fall-lucka #TODO check this class
      public:
	Hatch(std::string, Area*, Area*);
	virtual std::string getType() const;
	virtual bool isBlocked(Character*) const;
	virtual std::string blockMessage() const;
	virtual std::string passMessage() const;
    };


}
#endif
