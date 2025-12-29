SMOLSCRIPT_SRC := $(wildcard smolscript/*.c)
SMOLSCRIPT_OBJ := $(patsubst smolscript/%.c, build/%.o, $(SMOLSCRIPT_SRC))
SMOLSCRIPT_INC := -Ismolscript -Ilibs -I.

FLAGS := -std=c99 -Wall -Wextra -g



smolc: $(SMOLSCRIPT_OBJ) build
	cc -o smolc $(SMOLSCRIPT_OBJ) $(FLAGS)

build/%.o: smolscript/%.c
	cc -c $< -o $@ $(SMOLSCRIPT_INC) $(FLAGS)

build:
	mkdir -p build
