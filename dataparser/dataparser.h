#ifndef DATAPARSER_H
#define DATAPARSER_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<exception>

#include<map>
#include<utility>

#include<exception>

namespace hax
{
    std::string Read(const std::string filename) 
    {
        std::ifstream file(filename.c_str());
        if(!file)
        {
           return "TODO throw exception"; //TODO throw exception
        }
        std::string buf;
        std::string line;
        while(std::getline(file,line))
        {
            buf+=line;   
        }
        return buf;

    };
class Primitive //TODO should they inherit the other way around so that prim doesn't have to have the map but the data has a empty value instead ???
    {
        /* TODO reintegrate the exception
        class ConversionException : public std::exception
        {
            std::string msg;
            ConversionException(const std::string msg) throw()
            {
                this->msg = msg;
            }; 
            ~ConversionException() throw() //NOTE can't religh
            {
            };
            virtual const char* what() const throw()
            {
                return "Conversion Exception: "+msg.c_str();
            };
        };
        */
            std::string value; //the value for the node (set only if primitive)
        public:
            Primitive(const std::string value)
            {
                this->value = value;
            };
            operator int() //TODO manage conversionexception's
            {
                std::stringstream ss(this->value);
                int t;
                ss >> t;
                return t;
            }
            operator std::string()
            {
                return this->value; 
            }
            operator bool()
            {
                if(string::tolower(this->value).compare("true"))
                    return true;
                else if(string::tolower(this->value).compare("false"))
                    return false;
                else
                    return false;
                    //throw ConversionException(this->value+" is not considered a boolean");
            }

    };

    class Node
    {
        std::map<std::string,Node> nodes;
        std::map<std::string,Primitive> values;
        //TODO handle arrays ??

            void CheckForNotFound(std::size_t,std::size_t);

            virtual Node GetNode(const std::string id) const
            {
                return nodes[id];
            };
            virtual Primitive GetPrimitive(const std::string id) const
            {
                return values[id];
            };

        public:
            //dependency injection, doesn't change state after constructed
            Node(const std::string); //read the data and send the things to map<.,.> directly and then throw away the data

            //virtual std::string GetNode(const std::string) const;

            virtual Node operator[](const std::string id) const
            {
                return this->GetNode(id);
            };
            virtual Primitive operator()(const std::string id) const
            {
                return this->GetPrimitive(id);    
            };
         
    };
};
int main(int argc, const char* argv[]);


#endif

