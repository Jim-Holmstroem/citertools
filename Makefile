HEADER_DIR=./include/cit/

all: test

test: test/argmax

test/argmax: test/argmax.cpp $(HEADER_DIR)argmax.h $(HEADER_DIR)definition.h $(HEADER_DIR)identity.h
	g++ -o test/argmax -std=c++0x -Wall test/argmax.cpp -Iinclude/

