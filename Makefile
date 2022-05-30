
buggy: buggy.o
	gcc buggy.o -o buggy
	mkdir build
	mv buggy build
	rm -f buggy.o

buggy.o: 
	gcc -c src/main.cpp -I inc -o buggy.o

clean: 
	rm -rf build
