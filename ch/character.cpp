#include "character.h"
#include "../area/area.h" //included here since there are only Area pointers
#include "../area/indoor.h"
#include "../obj/object.h" //same reason as above
#include "../obj/coin.h"
#include "../area/route.h"

hax::Character::Character(){
    name = "defaultName";
    curArea = NULL;
    inventory = new Pocket();
    curContainer = inventory;
}
hax::Character::Character(std::string n){
    name = n;
    curArea = NULL;
    inventory = new Pocket(name, 5);
    curContainer = inventory;

    myWallet.add(new NdCoin()); //1000 kr

/*
//works perfectly, just too many debug prints
Coin values[] = {PtCoin(), AuCoin(), AgCoin(), CuCoin(), FeCoin(), NiCoin()};
//coin worth: 100, 50, 20, 10, 5, 1 kr
while(myWallet.getPrice() < 200){
myWallet.add(new Coin( values[rand()%len(values)] ));
}
*/
}
hax::Character::Character(const Character& ch){
    std::cout <<"Character::copy constructor"<< std::endl;
    curArea = ch.curArea;
    align = ch.align;
    //race = ch.getType();
    name = ch.name;
    curHp = ch.curHp;
    maxHp = ch.maxHp;
    strength = ch.strength;
//    inventory = new Pocket(ch.inventory); //TODO ok?
}
hax::Character::~Character(){
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
        curArea->leave(this); //remove the pointer to this from Area::vector<Character*>
    }
}

/*hax::Character& hax::Character::operator=(const Character& ch){
  return *this;
  };*/

bool hax::Character::alignment() const{
    return align;
}
std::string hax::Character::getName() const{
    return name;
}
int hax::Character::getcurHp() const{
    return curHp;
}
int hax::Character::getmaxHp() const{
    return maxHp;
}
int hax::Character::getStr() const{
    return strength;
}
int hax::Character::totWeight() const{
    return(this->weight + inventory->getWeight());
}
bool hax::Character::hasObject(Object* const ob) const{
    return inventory->hasObject(ob);
}
void hax::Character::view_stats() const{
#ifdef DEBUG
    std::cout << name <<" the "<< getType() << std::endl;
    std::cout <<"HP: "<< curHp <<"/"<< maxHp << std::endl;
    std::cout <<"Strength: "<< strength << std::endl;
    std::cout <<"Weight: "<< totWeight() << std::endl;
    std::cout <<"Total money: "<< myWallet.getPrice() <<" h@x"<< std::endl;
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
    oss <<"Total money: "<< myWallet.getPrice() <<" h@x"<< std::endl;
    hax::log.write(oss.str());
#endif
}
void hax::Character::view_area() const{
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
bool hax::Character::move_obj(std::string objName, std::string source, std::string destination){
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
void hax::Character::view_inventory() const{ //print objects in all containers
//TODO
}
void hax::Character::view_curContainer() const{
    std::cout << curContainer->description() <<" (maxW"<< curContainer->hold_weight() <<"/maxV"<< curContainer->hold_volume() <<"): "<< curContainer->contents() << std::endl;
}
bool hax::Character::change_curContainer(std::string objName){ //similar to linux command "cd", simple version
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
bool hax::Character::go(std::string routeName){
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
bool hax::Character::go(const Route& ro){ //TODO Route is passed by reference, what is the difference with pass by pointer? will virtual calls work?
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
bool hax::Character::go_random(){
    unsigned int routeNr = rand() % curArea->exits.size();
    MapRoute::const_iterator it = curArea->exits.begin();
    for(unsigned int i=0; i<routeNr; i++){
        it++;
    }
    return(this->go( *(it->second) ));
}
bool hax::Character::fight(std::string name){
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
void hax::Character::fight(Character* ch){
    attack(ch);
//TODO
/*        char msg[512];
          sprintf(msg,"%s has %iHP left",ch->name.c_str(),ch->curHp);
          hax::log.write(msg);
*/
    std::cout<< ch->name <<" has " << ch->curHp <<" HP left."<< std::endl << std::endl;
    if(ch->curHp <= 0){
        return;
//        delete ch; //character is killed
//        ch = NULL;
    }else{
        ch->fight(this); //continue if other character is not killed
    }
}
bool hax::Character::fight_random(){
    const SerializableVector<Character*> charsInArea = curArea->chars();
    unsigned int totChars = charsInArea.size();
    if(totChars == 1){ //no one to fight
        return false;
    }
    Character* enemy = charsInArea[rand() % totChars];
    while(this == enemy){ //protection against fighting myself
        enemy = charsInArea[rand() % totChars];
    }
    fight(enemy);
    return true;
}
bool hax::Character::pick_up(std::string objName){
    Object* ob = curArea->getObject(objName);
    if(ob != NULL){ //object found in curArea
        if(ob->getWeight() < 0){ //Obstacle should not be picked up and has weight = -1
            std::cout <<"Only Chuck Norris can pick up Obstacles!"<< std::endl;
            return false;
        }else if(curArea->drop(ob)){ //object picked up (removed from curArea), now add to correct container (no need to consider this in drop since cannot drop money)
            if(ob->getName() == "coin"){
                myWallet.add(ob);
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
bool hax::Character::drop(std::string objName){
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
bool hax::Character::buy(std::string objName){
    Shop* shopArea = dynamic_cast<Shop*>(curArea);
    if(shopArea != NULL){
        Object* purchase = shopArea->sell(myWallet.getPrice(), objName);
        if(purchase != NULL){
            curContainer->add(purchase);
            myWallet -= purchase->getPrice();
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
bool hax::Character::sell(std::string objName){
    Shop* shopArea = dynamic_cast<Shop*>(curArea);
    if(shopArea != NULL){
        Object* sale = curContainer->getObject(objName);
        if(sale != NULL){
            shopArea->buy(sale);
            curContainer->remove(sale);
            myWallet += sale->getPrice();
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
bool hax::Character::rest(){
    return(curArea->rest(this));
}
void hax::Character::ToString(std::ostream& out) const{
    std::ostringstream oss;
    oss << getType() <<" "<< name <<" "<< curHp <<" "<< maxHp <<" "<< strength <<" "<< weight;
    out << oss.str();
    out << curArea << std::endl; //OBS the address of curArea is written to the stream
    out << inventory << std::endl;
    out << &myWallet << std::endl;
    out << curContainer << std::endl;
};
void hax::Character::FromString(std::istream& in){
}; //TODO

/*
  void hax::Character::check(Object* ob){ //TODO
  if( curArea->hasObject(ob) ){
  Object* interact = *(curArea->findObject(ob));
  interact->check(*(this->inventory));
  }
  }
*/





hax::Character::Wallet::Wallet(){
    name = "wallet";
    descr = "";
    weight = 1;
    volume = 1;
    price = 0;
}
hax::Character::Wallet& hax::Character::Wallet::operator+=(const int profit){ //add Coins equal to int value
    *this -= (-profit);
    return *this;
}
hax::Character::Wallet& hax::Character::Wallet::operator-=(const int cost){ //TODO return NULL first if cost > this->getPrice() ???
//pop coins until amount is larger than cost then add new Coins for the change (växel) from shop
    int payment = 0;
    while(payment < cost){
        Object* mynt = vec_obj.front(); //TODO change to Coin* when implement template
        this->remove(mynt);
#ifdef DEBUG
        std::cout <<"Removed "<< mynt->description() << std::endl;
#endif
        payment += mynt->getPrice();
        delete mynt; //give to shopkeeper, gone forever
    }
    int change = payment - cost;

    Coin moneyValue[] = {NdCoin(), CsCoin(), PtCoin(), AuCoin(), AgCoin(), CuCoin(), FeCoin(), NiCoin()};
    for(size_t i=0; i < len(moneyValue); i++){
        int numCoin = change / moneyValue[i].getPrice(); //truncated division
        std::cout <<"numCoin = "<< numCoin << std::endl;
        change -= numCoin * moneyValue[i].getPrice();
        for(int j=0; j<numCoin; j++){
            this->add(new Coin(moneyValue[i]));
#ifdef DEBUG
            std::cout <<"Added "<< moneyValue[i].description() << std::endl;
#endif
        }
    }
    return *this;
}
int hax::Character::Wallet::hold_weight() const{
    return 1;
}
int hax::Character::Wallet::hold_volume() const{
    return 1000;
}
std::string hax::Character::Wallet::getType() const{
    return "wallet";
}



hax::Character::Pocket::Pocket(){
    name = "pocket";
    descr = "nobody's";
}
hax::Character::Pocket::Pocket(std::string owner, unsigned int maxSize){
    name = "pocket";
    descr = owner +"'s";
    weight = 1;
    volume = 1;
    this->maxSize = maxSize;
}
int hax::Character::Pocket::hold_weight() const{ //TODO hold_size() instead
    return 100;
}
int hax::Character::Pocket::hold_volume() const{
    return 100;
}
std::string hax::Character::Pocket::getType() const{
    return "pocket";
}
