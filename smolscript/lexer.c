#include "lexer.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void init_lexer(struct lexer* l, FILE* f)
{
  l->src = f;
  l->char_pos = 0;
  l->current_position = 0;
  l->line_pos = 0;
}

int peek(struct lexer* l)
{
  int c = fgetc(l->src);
  ungetc(c, l->src);
  return c;
}

int consume(struct lexer *l)
{
  int c = fgetc(l->src);
  if(c == '\n')
  {
    l->line_pos++;
    l->char_pos = 0;
  }
  else
  {
    l->char_pos++;
  }
  l->current_position++;
  return c;
}

void skipSpace(struct lexer *l)
{
  while(isspace(peek(l)))
  {
    consume(l);
  }
}

struct token next_token(struct lexer* l)
{

  skipSpace(l);

  int c = peek(l);
  if(c == -1)
    return (struct token){.type = KW_EOF};

  if(c == '!'){
    consume(l);
    return (struct token){
      .type = KW_BANG,
      .chr = l->char_pos,
      .line = l->line_pos
    };
  }

  if(c == '=')
  {
    consume(l);
    if(consume(l) == '>')
    {
      return (struct token){
        .type = KW_ARROW,
        .line = l->line_pos,
        .chr = l->char_pos
      };
    }
    else 
    {
      printf("[ERROR] Lexed \'=\' but no \'>\' \n");
      return (struct token){.type = LEX_ERR};
    }
  }

  if(isalpha(c))
  {
    return lex_str(l);
  }

  if(isdigit(c) || c == '-' || c == '+')
  {
    return lex_int(l);
  }

  return (struct token){.type = LEX_ERR};
}


struct token lex_str(struct lexer* l)
{
  char str[STR_LEN] = {0};
  int c = peek(l);
  int len = 0;  
  while(
    (len < STR_LEN) && 
    (isalpha(c) || isdigit(c))
  )
  {
    str[len++] = c;
    consume(l);
    c = peek(l);
  }

  struct token t = {
    .line = l->line_pos, 
    .chr = l->char_pos
  };

  strcpy(t.identifier, str);
  if(strcmp(str, "when") == 0)
  {
    t.type = KW_WHEN;
  }
  else if(strcmp(str, "true") == 0)
  {
    t.type = KW_TRUE;
  }
  else if(strcmp(str, "false") == 0)
  {
    t.type = KW_FALSE;
  }
  else if(strcmp(str, "set") == 0)
  {
    t.type = KW_SET;
  }
  else 
  {
    t.type = LIT_STR;
  }
  return t;
}

struct token lex_int(struct lexer* l)
{
  char str[STR_LEN] = {};
  int len = 0;
  char c = consume(l);
  do 
  {
    str[len++] = c;
    c = consume(l);
  }
  while(isdigit(c));

  //printf("[DEBUG] Reading int : %s\n", str);
  return (struct token)
    {
      .type = LIT_NUM,
      .chr = l->char_pos,
      .line = l->line_pos,
      .value = atoi(str)
    };
}

void print_token(struct token t)
{
  printf("[DEBUG] Token : {type : %d, char : %d, line : %d}\n", t.type, t.chr, t.line);
  if(t.type == LIT_STR)
  {
    printf("[DEBUG] Token : string value : %s\n", t.identifier);
  }

  if(t.type == LIT_NUM)
  {
    printf("[DEBUG] Token : int value : %d\n", t.value);
  }
}
