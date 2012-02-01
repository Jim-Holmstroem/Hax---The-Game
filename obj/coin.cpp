#include "coin.h"

hax::Coin::Coin(){
    name = "coin";
    weight = 0;
    volume = 1;
}
hax::Coin::Coin(const Coin& copy){
    name = copy.name;
    descr = copy.descr;
    weight = copy.weight;
    volume = copy.volume;
    price = copy.price;
}
std::string hax::Coin::getType() const{
    return "coin";
}
hax::NdCoin::NdCoin() : Coin(){
    descr = "neodymium";
    price = 1000; //value of coin
}
hax::CsCoin::CsCoin() : Coin(){
    descr = "cesium";
    price = 500;
}
hax::PtCoin::PtCoin() : Coin(){
    descr = "platinum";
    price = 100;
}
hax::AuCoin::AuCoin() : Coin(){
    descr = "gold";
//    weight = 3;
    price = 50;
}
hax::AgCoin::AgCoin() : Coin(){
    descr = "silver";
//    weight = 2;
    price = 20;
}
hax::CuCoin::CuCoin() : Coin(){
    descr = "copper";
//    weight = 1;
    price = 10;
}
hax::FeCoin::FeCoin() : Coin(){
    descr = "iron";
    price = 5;
}
hax::NiCoin::NiCoin() : Coin(){
    descr = "nickel";
    price = 1;
}
