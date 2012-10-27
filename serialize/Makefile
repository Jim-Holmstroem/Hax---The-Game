MOD = test iserializable iserializablecontainer serializablemap serializablevector serializablestring parsehelper heap

SRC = $(MOD:=.cpp)
HDR = $(MOD:=.h)
OBJ = $(MOD:=.o)

EXE = test_serialize

CXX = g++

CFLAG = -c -g -Wall -Wextra #-ftabstop=4 
LFLAG =

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(LFLAG) $(OBJ) -o $(EXE)

iserializable.o : iserializable.cpp iserializable.h
	$(CXX) $(CFLAG) iserializable.cpp -o iserializable.o
iserializablecontainer.o : iserializablecontainer.cpp iserializablecontainer.h
	$(CXX) $(CFLAG) iserializablecontainer.cpp -o iserializablecontainer.o
serializablemap.o : serializablemap.cpp serializablemap.h
	$(CXX) $(CFLAG) serializablemap.cpp -o serializablemap.o
serializablevector.o : serializablevector.cpp serializablevector.h
	$(CXX) $(CFLAG) serializablevector.cpp -o serializablevector.o
heap.o : heap.cpp heap.h
	$(CXX) $(CFLAG) heap.cpp -o heap.o 
parsehelper.o : parsehelper.cpp parsehelper.h
	$(CXX) $(CFLAG) parsehelper.cpp -o parsehelper.o
test.o : test.cpp test.h
	$(CXX) $(CFLAG) test.cpp -o test.o 
serializablestring.o : serializablestring.cpp serializablestring.h
	$(CXX) $(CFLAG) serializablestring.cpp -o serializablestring.o 
serializableinteger.o : serializableinteger.cpp serializableinteger.h
	$(CXX) $(CFLAG) serializableinteger.cpp -o serializableinteger.o 



clean:
	rm -f $(OBJ) $(EXE)

run:
	./$(EXE)
