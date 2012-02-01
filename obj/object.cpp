#include "object.h"

hax::Object::Object(){
    name = "object";
    descr = "default";
    weight = 0;
    volume = 0;
    price = 0;
}
/*hax::Object::Object(const Object& ob){
  weight = ob.weight;
  volume = ob.volume;
  price = ob.price;
  name = ob.name;
  descr = ob.descr;
  }*/
hax::Object::~Object(){
    std::cout << "~Object: " << description() << " deleted" << std::endl;
}

int hax::Object::getWeight() const{
    return weight;
}
int hax::Object::getVolume() const{
    return volume;
}
int hax::Object::getPrice() const{
    return price;
}
std::string hax::Object::getName() const{ //TODO obsolete now that we have getType()
    return name;
}
std::string hax::Object::description() const{
    if(descr != ""){
        return (descr +" "+ name);
    }else{
        return name;
    }
}
void hax::Object::ToString(std::ostream& out) const{
    std::ostringstream oss;
    oss << name <<" "<< descr <<" "<< weight <<" "<< volume <<" "<< price << std::endl;
    out << oss.str();
};
void hax::Object::FromString(std::istream& in){
}; //TODO

hax::Key::Key(){
    name = getType();
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
std::string hax::Key::getType() const{
    return "key";
}
