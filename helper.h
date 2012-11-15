#ifndef HELPER_H
#define HELPER_H

#include<cstring>
#include<vector>

#define UNUSED(x) (void)(sizeof((x), 0))

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
