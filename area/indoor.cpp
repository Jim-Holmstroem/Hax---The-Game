#include<cstdio>
#include<cstdlib>
#include<fstream>
#include "indoor.h"
#include "../ch/character.h"
#include "../obj/container.h"

hax::Indoor::Indoor() : Area(){}
hax::Indoor::Indoor(std::string name) : Area(name){}
hax::Indoor::Indoor(const Indoor& in) : Area(in){}
hax::Indoor::~Indoor(){}
void hax::Indoor::enter(Character* ch)
{
    Area::enter(ch);
}
void hax::Indoor::leave(Character* ch)
{
    Area::leave(ch);
}
bool hax::Indoor::rest(Character* ch) //TODO mix in stamina
{
    Area::rest(ch);
    int maxHp = ch->getmaxHp();
    int heal = maxHp/5;
    ch->curHp += heal;
    std::cout << ch->getName() << " healed " << heal << " HP" << std::endl;
    if(ch->curHp > maxHp){
        ch->curHp = maxHp;
    }
    return true;
}
hax::Character* hax::Indoor::spawn()
{
    return NULL;
}
void hax::Indoor::ToString(std::ostream& out) const
{
    Area::ToString(out);
}
void hax::Indoor::FromString(std::istream& in)
{
    Area::FromString(in);
    if(in.peek() == '\n'){in.get();}
}
std::string hax::Indoor::getType() const{return "indoor";}


hax::Castle::Castle() : Indoor(){}
//låt en route vara "far away you see a door behind a statue of Mr T."
hax::Castle::Castle(std::string name) : Indoor(name)
{
    descr = "You are inside a huge castle.";
}
std::string hax::Castle::getType() const{return "castle";}


hax::School::School() : Indoor(){}
hax::School::School(std::string name) : Indoor(name)
{
    descr = "Here you can has some new gangsta skillz.";
}
std::string hax::School::getType() const{return "school";}


hax::Shop::Shop() : Indoor(){}
hax::Shop::Shop(std::string name) : Indoor(name)
{
    descr = "Here you can get some bling-bling.";
    goods = new Ground("Goods sold at " + name);

    for(int i=0; i<5; i++){
        goods->add(new Backpack()); //TODO put loop in derived Shop after testing Shop
    }
}
hax::Shop::~Shop()
{
    delete goods;
}
bool hax::Shop::buy(Object* sale)
{
    this->goods->add(sale);
    return true;
}
hax::Object* hax::Shop::sell(const int totalCash, std::string objName)
{
    Object* ware = this->goods->getObject(objName);
    if(ware != NULL && totalCash > ware->getPrice()){
        this->goods->remove(ware);
    }
    return ware;
}
std::string hax::Shop::description() const
{
    std::ostringstream oss;
    oss <<"Goods here: "<< goods->contents() << std::endl;
    return(Area::description() + oss.str());
}
void hax::Shop::ToString(std::ostream& out) const
{
    Indoor::ToString(out);
    out << goods <<":";
    serializeQueue.push(goods);
}
void hax::Shop::FromString(std::istream& in)
{
    Indoor::FromString(in);
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Shop::FromString" << std::endl;

    std::string data;
    std::getline(in,data,':');
    std::string goodsUID = data;
    goods = dynamic_cast<Ground*>(pointerTable[goodsUID]);
    dbg << "goods UID = " << goodsUID << " | goods new address = " << goods << std::endl;

    if(in.peek() == '\n'){in.get();}
    dbg.close();
}
std::string hax::Shop::getType() const{return "shop";}
