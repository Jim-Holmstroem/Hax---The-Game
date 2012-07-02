#ifndef PARSEHELPER_H
#define PARSEHELPER_H

#include<iostream>
#include<string>
#include<exception>
#include<stdexcept>

//TODO add namespace helper (and change whereever it's used)

#include "../area/area.h"
#include "../area/indoor.h"
#include "../area/outdoor.h"
#include "../area/road.h"
#include "../area/route.h"
#include "../ch/character.h"
#include "../ch/hero.h"
#include "../ch/human.h"
#include "../ch/undead.h"
#include "../obj/coin.h"
#include "../obj/container.h"
#include "../obj/object.h"
#include "../obj/ostacle.h"

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
    //NOTE fugly
    //list all non-abstract classes and their defaultcontructors
    
    //==== AREA ====
    if(type=="area")
    {
        return new area::area();
    }
    else if(type=="indoor")
    {
        return new indoor();
    }
    else if(type=="outdoor")
    {
        return new outdoor();
    }
    else if(type=="road")
    {
        return new road();
    }
    else if(type=="route")
    {
        return new route();
    }
    //==== CH ====
    else if(type=="character")
    {
        return new character();
    }
    else if(type=="hero")
    {
        return new hero();
    }
    else if(type=="human")
    {
        return new human();
    }
    else if(type=="undead")
    {
        return new undead();
    }
    //==== OBJ  ====
    else if(type=="coin")
    {
        return new coin();
    }
    else if(type=="container")
    {
        return new container();
    }
    else if(type=="object")
    {
        return new object();
    }
    else if(type=="obstacle")
    {
        return new obstacle();
    }
    else
    {
        throw type+" doesn't exist";
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
