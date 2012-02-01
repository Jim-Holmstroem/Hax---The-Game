MOD = dataparser 

SRC = $(MOD:=.cpp)
HDR = $(MOD:=.h)
OBJ = $(MOD:=.o)

PRGM = dataparser

CFLAG = -c -Wall -g
LFLAG = 


all:
	make $(PRGM)

$(PRGM): $(OBJ) Makefile
	g++ $(OBJ) $(LFLAG) -o $(PRGM)

dataparser.o: dataparser.cpp dataparser.h Makefile
	g++ $(CFLAG) dataparser.cpp -o dataparser.o

clean:
	rm -f $(OBJ) $(PRGM)

run:
	./$(PRGM)
