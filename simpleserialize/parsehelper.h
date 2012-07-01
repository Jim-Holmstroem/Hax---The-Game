#ifndef PARSEHELPER_H
#define PARSEHELPER_H

#include<iostream>
#include<string>
#include<exception>
#include<stdexcept>

//TODO add namespace helper (and change whereever it's used)

namespace hax {

struct ParseException : public std::runtime_error
{
    ParseException(const std::string& msg):std::runtime_error(msg){};
    ParseException(const std::string& token,const std::string& found)
        :std::runtime_error("missing '"+token+"',instead found: '"+found+"'"){};

};

inline void checkToken(std::istream& in,const std::string& check) throw()
{
    std::string token;
    in >> token;
    if(token.compare(check)!=0)
    {
        std::cerr << "fuu:" << in.rdbuf();
        throw ParseException(check,token);       
    }
};

void* Generate(const std::string& type)
{
    //list all non-abstract classes and their defaultcontructors
    if(type=="")
    {
    
    }else if(type=="out")
    {
    
    }
};

/*
inline bool peekToken(std::istream& in,const std::string& check)
{
    std::string token(in.peek());
    return token.compare(check)==0;
}
*/

}

#endif
