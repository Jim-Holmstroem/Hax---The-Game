MOD = helper game level area indoor outdoor route road character hero human undead object container obstacle coin logger sprite multisprite 

SRC = $(MOD:=.cpp)
HDR = $(MOD:=.h)
OBJ = $(MOD:=.o)

PRGM = hax

#built-in variable for directory search
VPATH = world:ch:area:obj:ncurses

# built-in variable for g++ compiler flags
CXXFLAGS = -g -Wall -Wextra -Wno-missing-field-initializers -Wno-write-strings
# The supresed warnings:
# 	missing-field-initializers - strct = {0} actually zeroes the struct
# 	write-strings - method("string") shouldn't be any problems.


LFLAGS = -lncurses -lmenu

all:
	make -j $(PRGM)

$(PRGM): $(OBJ) Makefile
	$(CXX) $(OBJ) $(LFLAGS) -o $(PRGM)

#game.o: game.cpp game.h Makefile
#level.o: level.cpp level.h Makefile

clean:
	rm -f $(OBJ) $(PRGM)
run:
	./$(PRGM)
