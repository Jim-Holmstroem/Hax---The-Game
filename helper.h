#ifndef HELPER_H
#define HELPER_H

#include<cstring>
#include<vector>

//can have problem with violate variables, luckely we don't use them :)
//REMEMBER add UNUSED __only__ when it logically not being in use.
#define UNUSED(x) (void)(x)

namespace hax{

std::vector<std::string> split(std::string input){
    std::vector<std::string> words;

    std::istringstream iss(input);
    do{
        std::string word;
        iss >> word; //read a word
        words.push_back(word); //add the word to words
    }while(iss);//while not empty

    words.pop_back(); //delete the ENTER that followed to input

    return words;
};

}
#endif
