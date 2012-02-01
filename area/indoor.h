#ifndef INDOOR_H
#define INDOOR_H
#include "area.h"

namespace hax{

    class Ground; //forward declaration

    class Indoor : public Area{
      public:
        Indoor();
        Indoor(std::string);
        Indoor(const Indoor&);
        ~Indoor();

//        Indoor& operator=(const Indoor&);
        void enter(Character*);
        void leave(Character*);
        virtual bool rest(Character*);
	virtual Character* spawn();

      protected:
      private:
    };

    class Castle : public Indoor{
      public:
        Castle();
        Castle(std::string);
	virtual std::string getType() const;
    };

    class School : public Indoor{
      public:
        School();
        School(std::string);
	virtual std::string getType() const;
    };
    class Shop : public Indoor{
      public:
	Shop(std::string);
	~Shop();
	virtual std::string getType() const;
	bool buy(Object*); // buy/sell operates like pick_up/drop
	Object* sell(const int, std::string);
	virtual std::string description() const;

      private:
	Ground* goods; //items for sale
    };
}
#endif
