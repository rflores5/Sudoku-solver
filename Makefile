all: main

main: main.cpp puzzle.h
	g++ main.cpp -o main

clean: 
	rm *.o main
