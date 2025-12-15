smolc: main.cpp build/compiler.o build/compiler.o build/binaries.o
	g++ main.cpp build/*.o -Wall -Wextra -o smolc -g  -I./

build/compiler.o: compiler.cpp compiler.hpp
	g++ compiler.cpp -Wall -Wextra -c -g -o build/compiler.o -I./

build/binaries.o: binaries.cpp binaries.hpp compiler.hpp
	g++ binaries.cpp -Wall -Wextra -g -o build/binaries.o -I./ -c 

