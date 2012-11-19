#ifndef HELPER_H
#define HELPER_H

#include<string>
#include<vector>

//can have problem with violate variables, luckely we don't use them :)
//REMEMBER add UNUSED __only__ when it logically not being in use.
#define UNUSED(x) (void)(x)

namespace hax{
//http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
std::vector<std::string> split(const std::string& s, char delim);
std::vector<std::string> split(const std::string& s);

}
#endif