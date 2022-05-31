
buggy: buggy.o
	g++ buggy.o -o buggy -v
	mkdir build
	mv buggy build
	rm -f buggy.o

buggy.o: 
	g++ -c src/main.cpp  -I inc -o buggy.o -std=c++11

clean: 
	rm -rf build
