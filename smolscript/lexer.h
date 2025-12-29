#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#define STR_LEN 255

enum lexem_type 
{
  KW_WHEN,
  KW_ARROW,
  KW_BANG,
  KW_TRUE,
  KW_FALSE,
  KW_SET,
  LIT_NUM,
  LIT_STR,
  LEX_ERR,
  KW_EOF
};

struct lexer
{
  FILE* src;
  int current_position;
  int char_pos;
  int line_pos;
};

struct token
{
  enum lexem_type type;
  union{
    char identifier[STR_LEN];
    int value;
  };
  int line;
  int chr;
};

//lexer api
void init_lexer(struct lexer* l, FILE* f);
int peek(struct lexer* l);
int consume(struct lexer* l);

//lexers
void skipSpace(struct lexer* l);
struct token next_token(struct lexer* l);
struct token lex_str(struct lexer* l);
struct token lex_int(struct lexer* l);

void print_token(struct token t);

#endif
