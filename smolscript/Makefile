smolc: main.cpp build/compiler.o
	g++ main.cpp build/compiler.o -Wall -Wextra -o smolc -g

build/compiler.o: compiler.cpp compiler.hpp
	g++ compiler.cpp -Wall -Wextra -c -g
