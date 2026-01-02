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
      return (struct token){.type = LEX_ERR};
    }
  }

  if(isalpha(c))
  {
    return lex_str(l);
  }

  if(isdigit(c) || c == '-' || c == '+')
  {
    int is_img = 0;
    long pos = ftell(l->src);
    struct token img = lex_img(l, &is_img);
    if(is_img == 1) return img;
    fseek(l->src, pos, SEEK_SET);
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
    (isalpha(c) || isdigit(c) || c == '_')
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
  else if(strcmp(str, "define") == 0)
  {
    t.type = KW_DEFINE;
  }
  else if(strcmp(str, "load") == 0)
  {
    t.type = KW_LOAD;
  }
  else if(strcmp(str, "image") == 0)
  {
    t.type = KW_IMAGE;
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
  if(peek(l) == '-' || peek(l) == '+')
  {
    str[len++] = consume(l);
  }
  while(isdigit( peek(l) ))
  {
    str[len++] = consume(l);
  }
  return (struct token)
    {
      .type = LIT_NUM,
      .chr = l->char_pos,
      .line = l->line_pos,
      .value = atoi(str)
    };
}

struct token lex_img(struct lexer *l, int* success)
{
  struct token img;
  img.line = l->line_pos;
  img.chr = l->char_pos;
  img.type = LIT_IMG;
  
  *success = 1;

  for(int i = 0; i < 16*16; i++)
  {
    skipSpace(l);
    int c = consume(l);
    if(!isdigit(c) && c != '.')
    {
      img.type = LEX_ERR; 
      *success = 0;
      return img;
    }
    else 
    { 
      img.img[i] = (c == '.' ? 0 : c - '0');
    }
  }

  return img;
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
