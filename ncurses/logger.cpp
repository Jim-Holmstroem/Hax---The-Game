#include "logger.h"

namespace hax{
    Logger log;
}

std::vector<std::string> hax::Logger::wordWrap(std::string str,size_t width){
    std::vector<std::string> lines;

    size_t curWidth = width;
    size_t lastPos = 0;

    while(curWidth < str.length()){
        std::string::size_type spacePos = str.rfind(' ',curWidth);
        if(spacePos == std::string::npos){
            spacePos = str.find(' ',curWidth);
        }
        if(spacePos != std::string::npos){
           lines.push_back(str.substr(lastPos,spacePos-lastPos));
           // str[spacePos] = '\n';
            curWidth = spacePos + width + 1;

            lastPos = spacePos+1;
        }
    }
    lines.push_back(str.substr(lastPos,std::string::npos));
    return lines;
};

void hax::Logger::write(char * msg){
    std::string str(msg);
    write(str);
};

void hax::Logger::write(std::string msg){
    std::vector<std::string> lines = wordWrap(msg,width);
    lines.push_back(std::string(width,'-'));
    std::string blank(width,' ');

    std::vector<std::string>::iterator it = lines.begin();
    std::vector<std::string>::iterator stop = lines.end();
    while(it!=stop){
        mvprintw(pos,COLS-width,blank.c_str());//clear line
        mvprintw(pos,COLS-width,it->c_str());//write on line
        ++pos;
        pos%=(LINES-2);
        ++it;
    }
    refresh();
};
        
void hax::Logger::clearLog(){
    std::string str(width,' ');
    for(int i=0;i<LINES-2;i++){
        mvprintw(i,COLS-width,str.c_str());
    }
    pos = 0;
};

hax::Logger::Logger(){
    width=40;
    pos=0;
};
   
