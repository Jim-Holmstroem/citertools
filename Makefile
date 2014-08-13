HEADER_DIR=./include/cit/

all: argmax argmin map composition2 io thread_pool zip

argmax: test/argmax
argmin: test/argmin
functional: test/functional
composition: test/composition
map: test/map
composition2: test/composition2
zip: test/zip
io: test/io
thread_pool: test/thread_pool

test/argmax: test/argmax.cpp $(HEADER_DIR)argmax.h $(HEADER_DIR)definition.h $(HEADER_DIR)identity.h
	g++ -o test/argmax -std=c++0x -Wall test/argmax.cpp -Iinclude/

test/argmin: test/argmin.cpp $(HEADER_DIR)argmin.h $(HEADER_DIR)definition.h $(HEADER_DIR)identity.h $(HEADER_DIR)composition.h
	g++ -o test/argmin -std=c++0x -Wall test/argmin.cpp -Iinclude/

#test/functional: test/functional.cpp $(HEADER_DIR)functional.h $(HEADER_DIR)definition.h
#	g++ -o test/functional -std=c++0x -Wall test/functional.cpp -Iinclude/

#test/composition: test/composition.cpp $(HEADER_DIR)composition.h $(HEADER_DIR)definition.h
#	g++ -o test/composition -std=c++0x -Wall test/composition.cpp -Iinclude/

test/map: test/map.cpp $(HEADER_DIR)map.h $(HEADER_DIR)tuple.h $(HEADER_DIR)definition.h
	g++ -o test/map -std=c++0x -Wall test/map.cpp -Iinclude/

test/composition2: test/composition2.cpp $(HEADER_DIR)composition2.h $(HEADER_DIR)definition.h
	g++ -o test/composition2 -std=c++0x -Wall test/composition2.cpp -Iinclude/

test/zip: test/zip.cpp $(HEADER_DIR)zip.h $(HEADER_DIR)definition.h
	g++ -o test/zip -std=c++0x -Wall test/zip.cpp -Iinclude/

test/io: test/io.cpp $(HEADER_DIR)io.h $(HEADER_DIR)definition.h
	g++ -o test/io -std=c++0x -Wall test/io.cpp -Iinclude/

test/thread_pool: test/thread_pool.cpp $(HEADER_DIR)thread/pool.h $(HEADER_DIR)definition.h
	g++ -o test/thread_pool -pthread -std=c++0x -Wall test/thread_pool.cpp -Iinclude/

