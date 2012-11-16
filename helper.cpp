#include"helper.h"
#include<sstream>
//http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
std::vector<std::string> hax::split(const std::string& s, char delim){
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;

    while(std::getline(ss, item, delim)){
        elems.push_back(item);
    }

    return elems;
}
std::vector<std::string> hax::split(const std::string& s){
    return split(s, ' '); 
}
