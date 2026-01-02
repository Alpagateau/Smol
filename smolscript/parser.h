#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "../smol.h"

struct parser
{
  struct lexer* lexer;  
  struct token last_token;
};

void init_parser(struct parser*, struct lexer*);
void expect(struct parser* p, enum lexem_type type);
void advance(struct parser* p);
bool accept(struct parser* p, enum lexem_type type);

struct condition parse_condition(struct parser* p);
struct setter parse_setter(struct parser* p);
struct command parse_command(struct parser* p);
struct rule parse_rule(struct parser* p);
struct image parse_image(struct parser* p);

struct program parse_program(struct parser* p);

void print_rule(struct rule r);

#endif
