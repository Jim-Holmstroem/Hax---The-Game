#include<cstdio>
#include<cstdlib>
#include<fstream>
#include "character.h"
#include "feeling.h"
#include "../area/area.h" //included here since there are only Area pointers
#include "../area/indoor.h"
#include "../obj/object.h" //same reason as above
#include "../obj/container.h"
#include "../obj/coin.h"
#include "../area/route.h"
#include "../serialize/simpleHeap.h"
#include "../helper.h"

hax::Character::Character()
{
    name = "defaultName";
    curArea = NULL;
    myWallet = NULL;
}
hax::Character::Character(std::string name, bool control)
{
    this->name = name;
    controllable = control;
    curArea = NULL;
    inventory = new Pocket(name, 5);
    curContainer = inventory;
    myWallet = new Wallet(name);
    myWallet->add(new NdCoin()); //1000 kr
/*
//works perfectly, just too many debug prints
Coin values[] = {PtCoin(), AuCoin(), AgCoin(), CuCoin(), FeCoin(), NiCoin()};
//coin worth: 100, 50, 20, 10, 5, 1 kr

while(myWallet.getPrice() < 200){
myWallet.add(new Coin( values[rand()%len(values)] ));
}
*/
}
hax::Character::Character(const Character& ch)
{
#ifdef DEBUG
    std::cout <<"Character::copy constructor"<< std::endl;
#endif
//    curArea = dynamic_cast<Area*>(allocateData(ch.curArea->getType(),ch.curArea->getName()));
    controllable = ch.controllable;
    name = ch.name;
    curHp = ch.curHp;
    maxHp = ch.maxHp;
    strength = ch.strength;
    inventory = new Pocket(*(ch.inventory));
    myWallet = new Wallet(*(ch.myWallet));
}
hax::Character::~Character()
{
    std::ostringstream oss;
    oss <<"~Character: "<< name <<" died"<< std::endl;
#ifdef DEBUG
    std::cout << oss.str();
#else
    hax::log.write(oss.str()); //TODO if ncurses unloads before last characters is cleaned then segfault
#endif

    if(curArea != NULL){ //if just create this Object without using enter() then curArea will be NULL, so delete causes seg fault

        //try to drop the carried objects, i.e. loot from dead character
        for(size_t i=0; i < inventory->size(); ){
            if(!drop( (*inventory)[i]->description() )){
                i++;
            }
        }
        delete inventory; //the objects not dropped will be deleted in ~Container()
        delete myWallet;
        curArea->leave(this); //remove the pointer to this from Area::vector<Character*>
    }
}

/*hax::Character& hax::Character::operator=(const Character& ch){
  return *this;
  };*/

Feeling hax::Character::getFeeling() const
{
    struct Distribution<4> dist = {{
        0.25f,
        0.25f,
        0.25f,
        0.25f,
    }};         
    return Feeling(dist);
}

bool hax::Character::isControllable() const{return controllable;}
std::string hax::Character::getName() const{return name;}
int hax::Character::getcurHp() const{return curHp;}
int hax::Character::getmaxHp() const{return maxHp;}
int hax::Character::getStr() const{return strength;}
int hax::Character::totWeight() const
{
    return(this->weight + inventory->getWeight());
}
bool hax::Character::hasObject(Object* const ob) const
{
    return inventory->hasObject(ob);
}
void hax::Character::view_stats() const
{
#ifdef DEBUG
    std::cout << name <<" the "<< getType() << std::endl;
    std::cout <<"HP: "<< curHp <<"/"<< maxHp << std::endl;
    std::cout <<"Strength: "<< strength << std::endl;
    std::cout <<"Weight: "<< totWeight() << std::endl;
    std::cout <<"Total money: "<< myWallet->getPrice() <<" h@x"<< std::endl;
#else
    hax::log.write(name +" the "+ getType());
    std::ostringstream oss;
    oss <<"HP: "<< curHp <<"/"<< maxHp;
    hax::log.write(oss.str());
    oss.str(""); //clear stream
    oss <<"Strength: "<< strength;
    hax::log.write(oss.str());
    oss.str("");
    oss <<"Weight: "<< totWeight();
    hax::log.write(oss.str());
    oss.str("");
    oss <<"Total money: "<< myWallet->getPrice() <<" h@x"<< std::endl;
    hax::log.write(oss.str());
#endif
}
void hax::Character::view_area() const
{
#ifdef DEBUG
    std::cout << curArea->description() << std::endl;
#else
    hax::log.write(curArea->description());
#endif
}
/*void hax::Character::look(std::string routeName){ //TODO
  std::cout << curArea->maptoRoute[routeName]->view() << std::endl;
  }*/
/*
bool hax::Character::move_obj(std::string objName, std::string destination){
    return move_obj(objName, curContainer->description(), destination);
}
*/
bool hax::Character::move_obj(std::string objName, std::string source, std::string destination)
{
    //move something from somewhere to somewhere
    std::map<std::string, Container*> allFolders = inventory->get_subContainers();
    if(allFolders.find(source) == allFolders.end()){
	std::cout <<"Source container not found!"<< std::endl;
	return false;
    }else if(allFolders.find(destination) == allFolders.end()){
	std::cout <<"Destination container not found!"<< std::endl;
	return false;
    }else if(allFolders[source]->getObject(objName) == NULL){
	std::cout <<"Target object not found in source container!"<< std::endl;
	return false;
    }else{
	Container* from = allFolders[source];
	Container* to = allFolders[destination];
	Object* target = from->getObject(objName);
	from->remove(target);
	to->add(target);
	return true;
    }
}
void hax::Character::view_inventory() const //print objects in all containers
{
    std::ostringstream oss;
    oss << *inventory;
#ifdef DEBUG
    std::cout << oss.str() << std::endl;
#else
    hax::log.write(oss.str());
#endif
}
void hax::Character::view_curContainer() const
{
    std::cout << curContainer->description() <<" (maxW"<< curContainer->hold_weight() <<"/maxV"<< curContainer->hold_volume() <<"): "<< curContainer->contents() << std::endl;
}
bool hax::Character::change_curContainer(std::string objName)
{ //similar to linux command "cd", simple version
    if(objName == "#"){ //go to root (class Pocket)
        curContainer = inventory;
        while(!folderStack.empty()){ //clear stack
            folderStack.pop();
        }
        return true;
    }else if(objName == ".."){ //go up one level in folderStack
        if(folderStack.empty()){ //stack is empty => go to root
            curContainer = inventory;
            return true;
        }else{
            curContainer = folderStack.top();
            folderStack.pop();
            return true;
        }
    }else{ //go down one level in folderStack
        Container* setContainer = dynamic_cast<Container*>(curContainer->getObject(objName));
        if(setContainer != NULL){ //object found and it is a container
            folderStack.push(curContainer); //add current folder before going down, so possible to go up later
            curContainer = setContainer;
            return true;
        }else{
            std::cout << objName <<" is not a container!"<< std::endl;
            return false;
        }
    }
}
bool hax::Character::go(std::string routeName)
{
    if(curArea != NULL){
        if( curArea->exits.find(routeName) != curArea->exits.end() ){
            return go( *(curArea->exits[routeName]) );
        }else{
            std::cout <<"Route does not exist!"<< std::endl;
            return false;
        }
    }else{
        std::cout <<"curArea is not set!"<< std::endl;
        return false;
    }
}
bool hax::Character::go(const Route& ro) //Route is passed by reference, what is the difference with pass by pointer? will virtual calls work?
{
    if(ro.isBlocked(this)){
        std::cout << this->name <<" is blocked when trying to go to "<< ro.nextArea->getName() <<" through a "<< ro.getType() <<". "<< ro.blockMessage() << std::endl;
        return false;
    }else{
        curArea->leave(this);
        curArea->neighbor(ro).enter(this);
        std::cout << name <<" enters "<< curArea->getName() <<" through a "<< ro.getType() <<". "<< ro.passMessage() << std::endl;
        return true;
    }
}
bool hax::Character::go_random()
{
    unsigned int routeNr = rand() % curArea->exits.size();
    MapRoute::const_iterator it = curArea->exits.begin();
    for(unsigned int i=0; i<routeNr; i++){
        it++;
    }
    return(this->go( *(it->second) ));
}

bool hax::Character::fight(std::string name)
{
    Character* enemy = curArea->getChar(name); //is NULL if not found
    if(enemy == NULL){
        std::cout <<"No character called "<< name <<" in this area."<< std::endl;
        return false;
    }else if(this == enemy){ //protection against fighting myself
        std::cout <<"You cannot fight yourself!"<< std::endl;
        return false;
    }else{
        fight(enemy);
        return true;
    }
}
void hax::Character::fight(Character* ch)
{
    attack(ch);
    std::cout<< ch->name <<" has " << ch->curHp <<" HP left."<< std::endl << std::endl;
    if(ch->curHp <= 0){
        delete ch; //character is killed
        return;
    }else{
        ch->fight(this); //continue if other character is not killed
    }
}
bool hax::Character::fight_random()
{
    Character* target = this;
    while(target == this) //protection against fighting myself
    {
        target = curArea->getAnotherRandomCharForInteraction();
        if(target == NULL){return false;}
    }
    fight(target);
    return true;
}
void hax::Character::attack(Character* enemy) //TODO
{
}
bool hax::Character::talk_to(std::string name)
{
    Character* target = curArea->getChar(name); //is NULL if not found
    if(target == NULL){
        std::cout <<"No `"<< name <<"` in this area.. man."<< std::endl;
        return false;
    }else if(this == target){ //protection against fighting myself
        std::cout <<"You cannot talk to yourself!"<< std::endl;
        return false;
    }else{
        talk_to(target);
        return true;
    }
}
void hax::Character::talk_to(Character* target)
{
    std::cout << getName() << ": I just don't know what to say man" << std::endl;
}
bool hax::Character::talk_to_random()
{
    Character* target = this;
    while(target == this) //protection against talking to myself
    {
        target = curArea->getAnotherRandomCharForInteraction();
        if(target == NULL){return false;}
    }
    talk_to(target);
    return true;
}
bool hax::Character::pick_up(std::string objName)
{
    Object* ob = curArea->getObject(objName);
    if(ob != NULL){ //object found in curArea
        if(ob->getWeight() < 0){ //Obstacle should not be picked up and has weight = -1
            std::cout <<"Only Chuck Norris can pick up Obstacles!"<< std::endl;
            return false;
        }else if(curArea->drop(ob)){ //object picked up (removed from curArea), now add to correct container (no need to consider this in drop since cannot drop money)
            if(ob->getType() == "coin"){
                myWallet->add(ob);
            }else{
                curContainer->add(ob);
            }
            std::cout << this->name <<" picked up "<< ob->description() <<" and put it in "<< curContainer->description() <<"."<< std::endl;
            return true;
        }else{ //should not happen
            std::cout << objName <<" cannot be picked up for some reason."<< std::endl;
            return false;
        }
    }else{
        std::cout << objName <<" not found in this area."<< std::endl;
        return false;
    }
}
bool hax::Character::drop(std::string objName)
{
    Object* ob = curContainer->getObject(objName);
    if(ob != NULL){ //object found in curContainer
        if( curArea->pick_up(ob) ){ //object dropped (added to curArea), now remove from container
            curContainer->remove(ob);
            std::cout << name <<" dropped "<< ob->description() <<" from "<< curContainer->description() <<"."<< std::endl;
            return true;
        }else{
            std::cout <<"Cannot drop "<< objName <<" here."<< std::endl;
            return false; //cannot drop object in curArea
        }
    }else{
        std::cout << objName <<" not found in current container!"<< std::endl;
        return false;
    }
}
bool hax::Character::buy(std::string objName)
{
    Shop* shopArea = dynamic_cast<Shop*>(curArea);
    if(shopArea != NULL){
        Object* purchase = shopArea->sell(myWallet->getPrice(), objName);
        if(purchase != NULL){
            curContainer->add(purchase);
            *myWallet -= purchase->getPrice();
            std::cout <<"You bought "<< objName <<" for "<< purchase->getPrice() <<" h@x and it was put in "<< curContainer->description() <<"."<< std::endl;
            return true;
        }else{
            std::cout <<"Object not found or you don't have enough money!"<< std::endl;
            return false;
        }
    }else{
        std::cout <<"You cannot buy anything here. This area is not a shop!"<< std::endl;
        return false;
    }
}
bool hax::Character::sell(std::string objName)
{
    Shop* shopArea = dynamic_cast<Shop*>(curArea);
    if(shopArea != NULL){
        Object* sale = curContainer->getObject(objName);
        if(sale != NULL){
            shopArea->buy(sale);
            curContainer->remove(sale);
            *myWallet += sale->getPrice();
            std::cout <<"You sold "<< objName <<" for "<< sale->getPrice() <<" h@x and it was taken from "<< curContainer->description() <<"."<< std::endl;
            return true;
        }else{
            std::cout <<"You do not have this item."<< std::endl;
            return false;
        }
    }else{
        std::cout <<"You cannot sell anything here. This area is not a shop!"<< std::endl;
        return false;
    }
}
bool hax::Character::rest()
{
    return(curArea->rest(this));
}
void hax::Character::give(Object* const obj)
{
    inventory->add(obj);
}
void hax::Character::ToString(std::ostream& out) const
{
    out << this <<":"<< getType() <<":"<< controllable <<":"<< name <<":"<< curHp <<":"<< maxHp <<":"<< strength <<":"<< weight <<":"<< curArea <<":"<< inventory <<":"<< curContainer <<":"<< myWallet <<":";
    serializeQueue.push(inventory);
    serializeQueue.push(myWallet);
}
void hax::Character::FromString(std::istream& in)
{
    std::ofstream dbg;
    dbg.open("load_debug.dat", std::ios::out | std::ios::app);
    dbg << "Character::FromString" << std::endl;

    std::string data;
    std::getline(in,data,':'); //read type
    if(data != getType())
    {
        dbg << "Type mismatch! Aborting load from file." << std::endl;
        dbg.close();
        return;
    }
    dbg << "Type = " << data << std::endl;

    std::getline(in,data,':');
    controllable = std::atoi(data.c_str());
    dbg << "Controllable = " << controllable << std::endl;

    std::getline(in,data,':');
    name = data;
    dbg << "Name = " << name << std::endl;

    std::getline(in,data,':');
    curHp = std::atoi(data.c_str());
    dbg << "Current HP = " << curHp << std::endl;

    std::getline(in,data,':');
    maxHp = std::atoi(data.c_str());
    dbg << "Max HP = " << maxHp << std::endl;

    std::getline(in,data,':');
    strength = std::atoi(data.c_str());
    dbg << "Strength = " << strength << std::endl;

    std::getline(in,data,':');
    weight = std::atoi(data.c_str());
    dbg << "Weight = " << weight << std::endl;

    std::getline(in,data,':');
    std::string curAreaUID = data;
    curArea = dynamic_cast<Area*>(pointerTable[curAreaUID]);
    dbg << "curArea UID = " << curAreaUID << " | curArea new address = " << curArea << std::endl;

    std::getline(in,data,':');
    std::string inventoryUID = data;
    inventory = dynamic_cast<Pocket*>(pointerTable[inventoryUID]);
    dbg << "inventory UID = " << inventoryUID << " | inventory new address = " << inventory << std::endl;

    std::getline(in,data,':');
    std::string curContainerUID = data;
    curContainer = dynamic_cast<Container*>(pointerTable[curContainerUID]);
    dbg << "curContainer UID = " << curContainerUID << " | curContainer new address = " << curContainer << std::endl;

    std::getline(in,data,':');
    std::string myWalletUID = data;
    myWallet = dynamic_cast<Wallet*>(pointerTable[myWalletUID]);
    dbg << "myWallet UID = " << myWalletUID << " | myWallet new address = " << myWallet << std::endl;

    if(in.peek() == '\n'){in.get();} //If the data being deserialized is a base class (ex. the class Character), then at this state the next char is a newline which has to be removed so the next lines are read properly. If an inherited class is being deserialized (ex. the class Human/Undead), then the line of data continues

    dbg.close();
}
std::string hax::Character::getType() const{return "character";}
/*
  void hax::Character::check(Object* ob){ //TODO
  if( curArea->hasObject(ob) ){
  Object* interact = *(curArea->findObject(ob));
  interact->check(*(this->inventory));
  }
  }
*/
void hax::Character::initStats(int curHp, int maxHp, int strength, int weight)
{
    this->curHp = curHp;
    this->maxHp = maxHp;
    this->strength = strength;
    this->weight = weight;
}
