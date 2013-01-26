HEADER_DIR=./include/cit/

all: test

test: test/argmax

test/argmax: test/argmax.cpp $(HEADER_DIR)argmax.h $(HEADER_DIR)definition.h
	g++ -o test/argmax -Wall test/argmax.cpp -Iinclude/

