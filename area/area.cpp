#include<cstdio>
#include<cstdlib>
#include<fstream>
#include "area.h"
#include "../ch/character.h"
#include "../obj/container.h"
#include "../helper.h"

hax::Area::Area()
{
//    gnd = new Ground(name);
}
hax::Area::Area(std::string n)
{
    name = n;
    gnd = new Ground(name);
}
hax::Area::Area(const Area& ar)
{
    std::cout << "Area::copy constructor" << std::endl;
    exits = ar.exits; //TODO cannot copy map like this
    vec_char = ar.vec_char;
    gnd = ar.gnd; //TODO cannot copy pointer like this
    name = ar.name;
    descr = ar.descr; //when no copy constructor here the descr set in the derived class will not be known by base class?
}
hax::Area::~Area()
{
    std::cout << "~Area: deleting " << name << "..." << std::endl;
    //delete Characters first since they could drop Objects that will be added to vec_obj
    while(!vec_char.empty()){
        delete vec_char.back();
//      vec_char.pop_back(); OBS this is done in ~Character
    }
    delete gnd;

    MapRoute::iterator it;
    for(it = exits.begin(); it != exits.end(); it++){
        delete it->second;
    }
    exits.clear();

    std::cout << name << " deleted" << std::endl;
}
std::string hax::Area::getName() const
{
    return name;
}
hax::SerializableVector<hax::Character*> hax::Area::chars() const
{
    return vec_char;
}
hax::Area& hax::Area::neighbor(const Route& ro) const
{
//TODO a command look north, return view and "this route leads to Hogwarts."
    return *(ro.nextArea);
}
std::string hax::Area::directions() const
{
    std::string myRoutes;
    //REMEMBER must use const_iterator in const function
    MapRoute::const_iterator it;
    for(it = exits.begin(); it != exits.end(); it++){
        myRoutes.append(it->second->getName() +" to "+ neighbor(*(it->second)).getName() +", ");
    }
//    std::for_each(exits.begin(), exits.end(), myRoutes.append(...);

    return myRoutes;
}
std::string hax::Area::description() const
{
    std::ostringstream oss;
    oss <<"You are in "<< name <<". "<< descr << std::endl;

    std::string curChar;
    SerializableVector<Character*>::const_iterator it;
    for(it = vec_char.begin(); it != vec_char.end(); it++){
        curChar.append( (*it)->getName() +" the "+ (*it)->getType() +", ");
    }
    oss <<"Characters here: "<< curChar << std::endl;
    oss <<"Objects here: "<< gnd->contents() << std::endl;
    oss <<"Routes: "<< directions() << std::endl;
    return (oss.str());
}
void hax::Area::addRoute(Route* ro)
{
    std::pair< MapRoute::iterator, bool > ret; //MapRoute is typedef
    ret = exits.insert( std::pair<std::string, Route*>(ro->getName(), ro) ); //will automatically check for key collisions
    if(ret.second == false){
        std::cout << "Route with name " << ret.first->second->getName() << " already exists" << std::endl;
        delete ro; //delete it since it will not be added to an Area, what will happen to the new Obstacle? TODO
    }
}
void hax::Area::enter(Character* ch)
{
//update current area, IGNORE currently this is done in Character so that the pointer can be protected
    ch->curArea = this;
    vec_char.push_back(ch);
}
void hax::Area::leave(Character* const ch)
{
    if(findChar(ch) != vec_char.end()){
        vec_char.erase( findChar(ch) );
    }
//TODO should set curArea = NULL?
}
//when a Character drops ob this Area will pick it up
bool hax::Area::pick_up(Object* const ob)
{
    gnd->add(ob);
    return true; //TODO derived classes can return false with virtual
}
//when a Character picks up ob this Area will drop it, i.e. giving ob to the Character
bool hax::Area::drop(Object* const ob)
{
    return gnd->remove(ob);
}
bool hax::Area::rest(Character* latmask)
{
    std::ostringstream oss;
    oss << latmask->getName()+" rests in "+name+"." << std::endl;
#ifdef DEBUG
    std::cout << oss.str();
#else
    hax::log.write(oss.str());
#endif
}
hax::Character* hax::Area::spawn()
{
    return NULL; //TODO
}
hax::Character* hax::Area::getChar(std::string name)
{
    //"it" is a pointer to a Character pointer
    SerializableVector<Character*>::iterator it;
    for(it = vec_char.begin(); it != vec_char.end(); it++){
        if((*it)->getName() == name){return *it;}
    }
    return NULL; //not found, return NULL
}
hax::SerializableVector<hax::Character*>::iterator hax::Area::findChar(Character* ch)
{
    SerializableVector<Character*>::iterator it;
    for(it = vec_char.begin(); it != vec_char.end(); it++){
        if(*it == ch){return it;}
    }
    return it; //not found, return vec_char.end()
}
bool hax::Area::hasChar(Character* const ch) const
{
    SerializableVector<Character*>::const_iterator it;
    for(it = vec_char.begin(); it != vec_char.end(); it++){
        if(*it == ch){return true;}
    }
    return false;
}
hax::Object* hax::Area::getObject(std::string name)
{
    return gnd->getObject(name);
}
hax::SerializableVector<hax::Object*>::iterator hax::Area::findObject(Object* ob)
{
    return gnd->findObject(ob);
}
bool hax::Area::hasObject(Object* const ob) const
{
    return gnd->hasObject(ob);
}
void hax::Area::ToString(std::ostream& out) const
{
    out << this <<":"<< getType() <<":"<< name <<":"<< descr <<":";
    for(size_t i=0; i<vec_char.size(); i++)
    {
        out << vec_char[i] << ":";
        serializeQueue.push(vec_char[i]); //we can do this because we know that the Characters are only owned by this Area
    }
    out << "end:";
    //REMEMBER must use const_iterator in const function
    MapRoute::const_iterator it;
    for(it = exits.begin(); it != exits.end(); it++)
    {
        out << it->second << ":";
        serializeQueue.push(it->second);
    }
    out << "end:";
    out << gnd <<":";
    serializeQueue.push(gnd);
}
void hax::Area::FromString(std::istream& in)
{
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Area::FromString" << std::endl;

    std::string data;
    std::getline(in,data,':'); //read type
    if(data != getType())
    {
        std::cerr << "Type mismatch! Aborting load from file." << std::endl;
        dbg.close();
        return;
    }
    dbg << "Type = " << data << std::endl;

    std::getline(in,data,':');
    name = data;
    dbg << "Name = " << name << std::endl;

    std::getline(in,data,':');
    descr = data;
    dbg << "Description = " << descr << std::endl;

    std::getline(in,data,':');
    while(data != "end")
    {
        vec_char.push_back(dynamic_cast<Character*>(pointerTable[data]));
        dbg << "Character UID = " << data << " | Character new address = " << vec_char.back() << std::endl;
        std::getline(in,data,':');
    }

    std::getline(in,data,':');
    while(data != "end")
    {
        Route* ro = dynamic_cast<Route*>(pointerTable[data]);
        exits.insert(std::pair<std::string,Route*>(ro->getName(), ro)); //OBS the Routes were allocated with constructor Route(std::string) so name is defined => OK to call getName()
        dbg << "Route UID = " << data << " | Route new address = " << ro << std::endl;
        std::getline(in,data,':');
    }

    std::getline(in,data,':');
    std::string gndUID = data;
    gnd = dynamic_cast<Ground*>(pointerTable[gndUID]);
    dbg << "Ground UID = " << gndUID << " | Ground new address = " << gnd << std::endl;

    if(in.peek() == '\n'){in.get();} //If the data being deserialized is a base class (ex. the class Area), then at this state the next char is a newline which has to be removed so the next lines are read properly. If an inherited class is being deserialized (ex. the class Indoor/Outdoor), then the line of data continues

    dbg.close();
}
std::string hax::Area::getType() const{return "area";}
