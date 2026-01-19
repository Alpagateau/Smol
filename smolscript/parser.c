#include "lexer.h"
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define DEBUG

void init_parser(struct parser* p, struct lexer* l)
{
  p->lexer = l;
  p->last_token = next_token(l);
}

void expect(struct parser* p, enum lexem_type type)
{
  if(p->last_token.type != type)
  {
    printf("[ERROR] Expected %d but got \n", type);
    print_token(p->last_token);
    exit(1);
  } 
}

bool accept(struct parser* p, enum lexem_type type)
{
  if(p->last_token.type == type)
  {
  advance(p);
    return true;
  }
  return false;
}

struct condition parse_condition(struct parser* p)
{
  #ifdef DEBUG
    printf("[DEBUG] Condition\n");
  #endif
  struct condition cond;
  if(p->last_token.type == KW_TRUE)
  {
    cond.constant = true;
    cond.inverted = false;
    cond.edge = false;
    advance(p);
    return cond;
  }
  else if(p->last_token.type == KW_FALSE)
  {
    cond.constant = true;
    cond.inverted = true;
    cond.edge = false;
    advance(p);
    return cond;
  }

  cond.inverted = accept(p, KW_BANG);
  expect(p, LIT_STR);
  strcpy(cond.name, p->last_token.identifier);
  advance(p);
  cond.edge = accept(p, KW_BANG);
  return cond;
}

struct setter parse_setter(struct parser *p)
{
  #ifdef DEBUG
    printf("[DEBUG] Setter\n");
  #endif
  struct setter s;
  expect(p, KW_SET);
  advance(p);
  expect(p, LIT_STR);
  strcpy(s.dest, p->last_token.identifier);
  advance(p);
  s.value = parse_condition(p);
  //advance(p);
  return s;
}

struct command parse_command(struct parser* p)
{
  #ifdef DEBUG
    printf("[DEBUG] Command\n");
  #endif
  struct command c;
  expect(p, LIT_STR);
  strcpy(c.name, p->last_token.identifier); 
  advance(p);
  c.arg_num = 0;
  while(
    p->last_token.type == LIT_NUM || 
    p->last_token.type == LIT_STR
  )
  {
    #ifdef DEBUG
      printf(
        "[DEBUG] args Next token type :  %d\n", 
        p->last_token.type
      );
    #endif
    c.args[c.arg_num++] = parse_argument(p);
  }

  #ifdef DEBUG
      printf(
        "[DEBUG] Next token type after : %d\n", 
        p->last_token.type
      );
    #endif

  return c;
}

void advance(struct parser* p)
{
  p->last_token = next_token(p->lexer);
}

struct rule parse_rule(struct parser *p)
{
  #ifdef DEBUG
    printf("[DEBUG] Rule\n");
  #endif
  struct rule r;
 
  expect(p, KW_WHEN);
  advance(p);
  r.condition = parse_condition(p);
  expect(p, KW_ARROW);
  advance(p);
  if(p->last_token.type == KW_SET)
  {
    r.is_setter = true;
    r.setter = parse_setter(p);
  }
  else
  {
    r.is_setter = false;
    r.command = parse_command(p);
  }
  return r;
}

struct image parse_image(struct parser *p)
{
  #ifdef DEBUG
    printf("[DEBUG] Image\n");
  #endif
  struct image img;
  expect(p, KW_IMAGE);
  advance(p);
  expect(p, LIT_STR);
  strcpy(img.name, p->last_token.identifier);
  advance(p);
  expect(p, LIT_IMG);
  memcpy(img.pxl, p->last_token.img, 16*16);
  advance(p);
  return img;
}

struct program parse_program(struct parser* p)
{
  #ifdef DEBUG
    printf("[DEBUG] Program\n");
  #endif
  struct program prgm = {}; 

  while(p->last_token.type != KW_EOF) 
  {  
    #ifdef DEBUG
      printf("[DEBUG] Next token type : %d\n", p->last_token.type);
    #endif
    switch (p->last_token.type) {
      case KW_IMAGE:
        prgm.images[prgm.img_nb++] = parse_image(p);
        break;
      case KW_DEFINE:
        {
          struct definer d = parse_definer(p);
          if(strcmp(d.name, "agents") == 0)
          {
            prgm.agent_nb = d.value;
            printf("Number of agents : %d\n", d.value);
          }
          break;
        }
      case KW_SPRITE:
        #ifdef DEBUG
          printf("[DEBUG] Sprite\n");
        #endif
        advance(p);
        expect(p, LIT_STR);
        strcpy(
          prgm.agents[prgm.agent_nb++], 
          p->last_token.identifier
        );
        advance(p);
        break;
      case KW_WHEN:
        prgm.rules[prgm.rule_nb++] = parse_rule(p);
        printf("[DEBUG] Condition %s => is setter : %d\n", 
               prgm.rules[prgm.rule_nb-1].condition.name,
               prgm.rules[prgm.rule_nb-1].is_setter
               );
        if(p->last_token.type == LIT_STR)
          printf("[DEBUG] String lit : %s at : %d %d\n", p->last_token.identifier, p->last_token.line, p->last_token.chr);
        break;
      case KW_EOF:
        break;
      default:
        //printf("What am i parsing ? %d\n", p->last_token.type);
        break;
    }
  }
  return prgm;
}

struct definer parse_definer(struct parser* p)
{
  #ifdef DEBUG
    printf("[DEBUG] Definer\n");
  #endif
  expect(p, KW_DEFINE);
  advance(p);

  struct definer d = {};

  expect(p, LIT_STR);
  strcpy(d.name, p->last_token.identifier);
  advance(p);
    
  expect(p, LIT_NUM);
  d.value = p->last_token.value;
  advance(p);
  return d;
}

struct argument parse_argument(struct parser *p)
{
  #ifdef DEBUG
    printf("[DEBUG] Argument\n");
  #endif
  struct argument a = {};
  if(p->last_token.type == LIT_STR)
  {
    a.agr_type = ARG_STR;
    strcpy(a.name, p->last_token.identifier);
    #ifdef DEBUG
      printf("[DEBUG] Read arg %s\n", a.name);
    #endif
  }
  else
  {
    expect(p, LIT_NUM);
    a.agr_type = ARG_INT;
    a.value = p->last_token.value;
    #ifdef DEBUG
      printf("[DEBUG] Read arg num %d\n", a.value);
    #endif
  }
  advance(p);
  return a;
}

void print_rule(struct rule r)
{
  printf("When %s (%d %d) then ", r.condition.name, r.condition.edge, r.condition.inverted);

  if(r.is_setter)
    printf("set %s to %s (%d, %d)\n", 
           r.setter.dest, r.setter.value.name, r.setter.value.edge, r.setter.value.inverted);
  else
    printf(" command %s with %d args\n", r.command.name, r.command.arg_num);
}
