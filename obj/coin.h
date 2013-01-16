#include "object.h"

namespace hax
{
    class Coin : public Object
    {
      public:
        Coin();
        Coin(const Coin&);
        virtual void ToString(std::ostream&) const;
        virtual void FromString(std::istream&);
	virtual std::string getType() const;
    };

    class NdCoin : public Coin{ //Nd = neodymium
      public:
        NdCoin();
    };
    class CsCoin : public Coin{ //Cs = cesium
      public:
        CsCoin();
    };
    class PtCoin : public Coin{ //Pt = platinum
      public:
        PtCoin();
    };
    class AuCoin : public Coin{ //Au = gold
      public:
        AuCoin();
    };
    class AgCoin : public Coin{ //Ag = silver
      public:
        AgCoin();
    };
    class CuCoin : public Coin{ //Cu = copper
      public:
        CuCoin();
    };
    class FeCoin : public Coin{ //Fe = iron
      public:
        FeCoin();
    };
    class NiCoin : public Coin{ //Ni = nickel
      public:
        NiCoin();
    };
}
