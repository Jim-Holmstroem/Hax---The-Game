#ifndef HEAP_H
#define HEAP_H

#include"iserializable.h"
#include"serializablemap.h"

#include<iostream>
#include<string>
#include<vector>

#include<string>
#include<sstream>
#include<stdexcept>

namespace hax
{
    class Description : public ISerializable
    {
      protected:
        std::string UID;
        std::string type;

        Description(std::string id,std::string tp)
        :UID(id)
        ,type(tp)
        {};

      public:
        Description(){}; //NOTE should never be used, except for gaining access to getType()    

        virtual void ToString(std::ostream& out) const
        {
            out << UID << ' ' << type << ' ';
        };
        virtual void FromString(std::istream& in)
        {
            in >> UID >> type;
        };
        
        static Description GetDescription(const ISerializable& obj)
        {
            return Description(obj.GetUID(),obj.getType());
        };

        bool operator<(const Description& d) const
        {
            return this->UID.compare(d.UID)<0;
        };
        std::string getType() const
        {
           return "Description"; 
        };

    };

    class Heap : public SerializableMap<Description,ISerializable*>
    {
      public:

        virtual void ElementToString(std::ostream& out, const value_type& elem) const
        {
            out << elem.first << ' ' << *elem.second << ' ';
        };
        virtual void ElementFromString(std::istream& in)
        {
            key_type keyData;
            in >> keyData;
            value_type elem(keyData, mapped_type());
            throw std::runtime_error("not implemented yet");
            //in >> elem.second;

            /*TODO ADD CODE HERE TO HANDLE HEAP-MAPPING*/

            content.insert(elem);
        };
        virtual std::string getType()
        {
            return "Heap"; //NOTE, will never be used
        };
      private:
        std::map<size_t,ISerializable*> convert;

    };

}

#endif
