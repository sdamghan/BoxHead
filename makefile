HDIR=./headers
SDIR=./SRC
BIN=./bin

CC=g++
CFLAGS=-lSDL -lSDL_image -lSDL_ttf

BoxHead : main.o functions.o
	g++ $(BIN)/*.o $(CFLAGS) -o BoxHead
main.o : $(SDIR)/main.cpp $(HDIR)/header.h
	g++ -c $(SDIR)/main.cpp $(CFLAGS) -o $(BIN)/main.o
functions.o : $(SDIR)/functions.cpp $(HDIR)/header.h
	g++ -c $(SDIR)/functions.cpp $(CFLAGS) -o $(BIN)/functions.o
clean:
	rm -rf $(BIN)
	mkdir $(BIN)
