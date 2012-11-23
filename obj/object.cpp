#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<queue>
#include "object.h"
#include "../helper.h"

hax::Object::Object()
{
    descr = "default";
    weight = 0;
    volume = 0;
    price = 0;
}
/*hax::Object::Object(const Object& ob){
  weight = ob.weight;
  volume = ob.volume;
  price = ob.price;
   descr = ob.descr;
  }*/
hax::Object::~Object()
{
    std::cout << "~Object: " << description() << " deleted" << std::endl;
}

int hax::Object::getWeight() const{return weight;}
int hax::Object::getVolume() const{return volume;}
int hax::Object::getPrice() const{return price;}
std::string hax::Object::description() const
{
    if(descr != ""){
        return (descr +" "+ getType());
    }else{
        return getType();
    }
}
void hax::Object::ToString(std::ostream& out) const
{
    out << this <<":"<< getType() <<":"<< descr <<":"<< weight <<":"<< volume <<":"<< price << std::endl;
}
void hax::Object::FromString(std::istream& in)
{
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Object::FromString" << std::endl;

    std::string data;
    std::getline(in,data); //read rest of line
    std::vector<std::string> parsedObj = split(data,':');
    std::queue<std::string> pQ;
    for(int i=0; i<parsedObj.size(); i++)
    {
        pQ.push(parsedObj[i]);
    }
    std::string type = pQ.front();
    pQ.pop();
    if(type != getType())
    {
        dbg << "Type mismatch! Aborting load from file." << std::endl;
        dbg.close();
        return;
    }
    descr = pQ.front();
    pQ.pop();
    dbg << "Description = " << descr << std::endl;

    weight = std::atoi(pQ.front().c_str());
    pQ.pop();
    dbg << "Weight = " << weight << std::endl;

    volume = std::atoi(pQ.front().c_str());
    pQ.pop();
    dbg << "Volume = " << volume << std::endl;

    price = std::atoi(pQ.front().c_str());
    pQ.pop();
    dbg << "Price = " << price << std::endl;

    dbg.close();
}
std::string hax::Object::getType() const{return "object";}


hax::Key::Key()
{
    const char* attr[] = { //attributes
        "orange",
        "rusty",
        "shiny",
	"gangsta",
	"master",
	"silver",
    };
    descr = attr[rand() % len(attr)];
    weight = 0;
    volume = 0;
    price = 0;
}
std::string hax::Key::getType() const{return "key";}
