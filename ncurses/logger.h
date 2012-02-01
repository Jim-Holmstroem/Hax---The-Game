#ifndef LOGGER_H
#define LOGGER_H

#include<string>
#include<cstdio>
#include<vector>
#include<ncurses.h>

namespace hax{

    class Logger{
      private:
        int pos; //where to write
        int width;
      protected:
	std::vector<std::string> wordWrap(std::string,size_t width);
      public:
        Logger();
        void clearLog();
        void write(std::string);
        void write(char*);
    };

}

#endif
