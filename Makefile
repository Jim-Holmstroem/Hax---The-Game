MOD = helper game level area indoor outdoor route road character human undead object container obstacle coin logger sprite multisprite simpleHeap

SRC = $(MOD:=.cpp)
HDR = $(MOD:=.h)
OBJ = $(MOD:=.o)

PRGM = hax

#built-in variable for directory search
VPATH = world:ch:area:obj:serialize:ncurses

# built-in variable for g++ compiler flags
CXXFLAGS = -g -Wall -Wextra -std=c++0x
LFLAGS = -lncurses -lmenu

all:
	make $(PRGM)

debug:
	make -f Makefile_debug
debug_clean:
	make -f Makefile_debug clean
debug_run:
	make -f Makefile_debug run


$(PRGM): $(OBJ) Makefile
	$(CXX) $(OBJ) $(LFLAGS) -o $(PRGM)

game.o: game.cpp game.h Makefile
level.o: level.cpp level.h Makefile

clean:
	rm -f $(OBJ) $(PRGM)
run:
	./$(PRGM)
