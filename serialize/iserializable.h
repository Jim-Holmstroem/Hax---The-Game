#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include<iostream>
#include<string>
#include<sstream>
#include"parsehelper.h"

namespace hax
{
    class ISerializable
    {
      protected:
        virtual void ToString(std::ostream& out) const = 0;
        virtual void FromString(std::istream& in) = 0;

      public:
        virtual std::string getType() const = 0;
        virtual std::string getUID() const
        {
            std::ostringstream uid;
            uid << this;
            return uid.str();
        };

        friend std::ostream& operator<<(std::ostream& out, ISerializable const& s)
        {
//            out << "{ ";
            s.ToString(out);
//            out << "}";
            return out;
        };

        friend std::istream& operator>>(std::istream& in,ISerializable& s)
        {
//            checkToken(in,"{");
            s.FromString(in);
//            checkToken(in,"}");
            return in;
        };
    };
}
#endif
