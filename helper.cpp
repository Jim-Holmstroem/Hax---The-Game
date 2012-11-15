#include"helper.h"
#include<sstream>

std::vector<std::string> hax::split(const std::string& s, char delim){
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;

    while(std::getline(ss, item, delim)){
        elems.push_back(item);
    }

    return elems;
}
inline std::vector<std::string> hax::split(const std::string& s){
    return split(s, ' '); 
}


