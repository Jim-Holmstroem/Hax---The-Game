#include "container.h"
#include "../obj/coin.h"
#include "../serialize/simpleHeap.h"

hax::Container::Container(){}
/*hax::Container::Container(const Container& co) : Object(co){
  vec_obj = co.vec_obj;
  }*/
hax::Container::~Container()
{
    std::cout << "~Container: emptying " << description() << "..." << std::endl;
    for(size_t i=0; i<vec_obj.size(); i++){
        delete vec_obj[i];
    }
    vec_obj.clear();
}

//TODO const version?
hax::Object* hax::Container::operator[](size_t i){return vec_obj[i];}

bool hax::Container::add(Object* const ob)
{
    if(weight+ob->getWeight() > hold_weight() || volume+ob->getVolume() > hold_volume()){
        return false;
    }else{
        vec_obj.push_back(ob);
        weight += ob->getWeight();
        volume += ob->getVolume();
        price += ob->getPrice();
        return true;
    }
}
bool hax::Container::remove(Object* const ob)
{
    if( findObject(ob) != vec_obj.end() ){
        vec_obj.erase(findObject(ob));
        weight -= ob->getWeight();
        volume -= ob->getVolume();
        price -= ob->getPrice();
        return true;
    }else{
        return false;
    }
}
size_t hax::Container::size() const{return vec_obj.size();}
bool hax::Container::empty() const{return vec_obj.empty();}
hax::Object* hax::Container::back(){return vec_obj.back();}
std::string hax::Container::contents() const
{
    std::ostringstream oss;
    SerializableVector<Object*>::const_iterator it;
    for(it = vec_obj.begin(); it != vec_obj.end(); it++){
        oss << (*it)->description() <<" (W"<< (*it)->getWeight() <<"/V"<< (*it)->getVolume() <<"/P"<< (*it)->getPrice() <<"), ";
    }
    return oss.str();
}
std::map<std::string, hax::Container*> hax::Container::get_subContainers()
{
    std::map<std::string, Container*> allFolders;
    std::queue<Container*> searchQueue;
    searchQueue.push(this);
    while(!searchQueue.empty()){
        Container* folder = searchQueue.front();
        searchQueue.pop();
        std::pair< std::map<std::string,Container*>::iterator, bool > ret;
        ret = allFolders.insert( std::pair<std::string,Container*>(folder->description(),folder) ); //will automatically check for key collisions
        int appendix = 0;
        while(ret.second == false){ //change appendix until find unique name
            std::ostringstream oss;
            oss << ++appendix;
            ret = allFolders.insert( std::pair<std::string,Container*>(folder->description()+oss.str(),folder) );
        }

        //add all objects which are Containers in this Container
        for(size_t i=0; i<folder->size(); i++){
            Container* test = dynamic_cast<Container*>( (*folder)[i] );
            if(test != NULL){
                searchQueue.push(test);
            }
        }
    }
    return allFolders;
}


hax::SerializableVector<hax::Object*>::iterator hax::Container::findObject(Object* const ob)
{
    SerializableVector<Object*>::iterator it;
    for(it = vec_obj.begin(); it != vec_obj.end(); it++){
        if(*it == ob){
            return it;
        }
    }
    return it; //not found, return vec_obj.end()
}
hax::Object* hax::Container::getObject(std::string objName)
{
    SerializableVector<Object*>::iterator it;
    for(it = vec_obj.begin(); it != vec_obj.end(); it++){
        if((*it)->description() == objName){
            return *it;
        }
    }
    return NULL; //not found, return NULL
}
bool hax::Container::hasObject(Object* const ob) const
{
    SerializableVector<Object*>::const_iterator it;
    for(it = vec_obj.begin(); it != vec_obj.end(); it++){
        if(*it == ob){
            return true;
        }
    }
    return false;
}
void hax::Container::ToString(std::ostream& out) const
{
    out << vec_obj;
}
void hax::Container::FromString(std::istream& in)
{
}//TODO
std::string hax::Container::getType() const{return "container";}


hax::Wallet::Wallet()
{
    descr = "";
    weight = 1;
    volume = 1;
    price = 0;
}
hax::Wallet& hax::Wallet::operator+=(const int profit) //add Coins equal to int value
{
    *this -= (-profit);
    return *this;
}
hax::Wallet& hax::Wallet::operator-=(const int cost) //TODO return NULL first if cost > this->getPrice()???
{
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
int hax::Wallet::hold_weight() const{return 1;}
int hax::Wallet::hold_volume() const{return 1000;}
std::string hax::Wallet::getType() const{return "wallet";}


hax::Pocket::Pocket()
{
    descr = "nobody's";
}
hax::Pocket::Pocket(std::string owner, unsigned int maxSize)
{
    descr = owner +"'s";
    weight = 1;
    volume = 1;
    this->maxSize = maxSize;
}
//TODO hold_size() instead
int hax::Pocket::hold_weight() const{return 100;}
int hax::Pocket::hold_volume() const{return 100;}
std::string hax::Pocket::getType() const{return "pocket";}


hax::Backpack::Backpack()
{
    const char* attr[] = { //attributes
        "gangsta",
        "old",
        "cool",
        "orange",
        "Hello Kitty"
    };
    descr = attr[rand() % len(attr)];
    weight = 1;
    volume = 2;
    price = 50;
}
int hax::Backpack::hold_weight() const{return 30;}
int hax::Backpack::hold_volume() const{return 10;}
std::string hax::Backpack::getType() const{return "backpack";}
