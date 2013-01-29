HEADER_DIR=./include/cit/

all: argmax functional composition map

argmax: test/argmax
functional: test/functional
composition: test/composition
map: test/map

test/argmax: test/argmax.cpp $(HEADER_DIR)argmax.h $(HEADER_DIR)definition.h $(HEADER_DIR)identity.h
	g++ -o test/argmax -std=c++0x -Wall test/argmax.cpp -Iinclude/

test/functional: test/functional.cpp $(HEADER_DIR)functional.h $(HEADER_DIR)definition.h
	g++ -o test/functional -std=c++0x -Wall test/functional.cpp -Iinclude/

test/composition: test/composition.cpp $(HEADER_DIR)composition.h $(HEADER_DIR)definition.h
	g++ -o test/composition -std=c++0x -Wall test/composition.cpp -Iinclude/

test/map: test/map.cpp $(HEADER_DIR)map.h $(HEADER_DIR)definition.h
	g++ -o test/map -std=c++0x -Wall test/map.cpp -Iinclude/

