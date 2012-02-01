#include"iserializable.h"
#include<string>

namespace hax{

    class SerializableString : public ISerializable
    {
      public:
        SerializableString(){};
        SerializableString(const std::string& str) : _str(str){};
        SerializableString(const char* str) : _str(str){};

        operator std::string() const{return _str;};

        virtual std::string getType() const{
            return "SerializableString";
        };

        bool operator<(const SerializableString& str) const{
            return _str.compare(str)<0;    
        };

      protected:
        virtual void ToString(std::ostream& out) const{
            out << _str << ' ';
        };
        virtual void FromString(std::istream& in){
            in >> _str;
        };

      private:
        std::string _str;
    };

}
