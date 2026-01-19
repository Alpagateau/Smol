SMOLSCRIPT_SRC := $(wildcard smolscript/*.c)
SMOLSCRIPT_OBJ := $(patsubst smolscript/%.c, build/%.o, $(SMOLSCRIPT_SRC))
SMOLSCRIPT_INC := -Ismolscript -Ilibs -I.

FLAGS := -std=c99 -Wall -Wextra -g

all: smolc libsmol.o test

test: main.c
	gcc -o test main.c libsmol.o -lraylib

smolc: $(SMOLSCRIPT_OBJ) build
	cc -o smolc $(SMOLSCRIPT_OBJ) $(FLAGS)

libsmol.o: smollib/smollib.c smollib/smollib.h
	cc -c -o libsmol.o smollib/smollib.c $(FLAGS) -Ismollib

build/%.o: smolscript/%.c 
	cc -c $< -o $@ $(SMOLSCRIPT_INC)  $(FLAGS) -DDEBUG

build:
	mkdir -p build
