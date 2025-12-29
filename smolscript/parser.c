#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include "parser.h"

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
  struct condition cond;
  cond.inverted = accept(p, KW_BANG);
  expect(p, LIT_STR);
  strcpy(cond.name, p->last_token.identifier);
  advance(p);
  cond.edge = accept(p, KW_BANG);
  return cond;
}

struct setter parse_setter(struct parser *p)
{
  struct setter s;
  expect(p, KW_SET);
  advance(p);
  expect(p, LIT_STR);
  strcpy(s.dest, p->last_token.identifier);
  advance(p);
  s.value = parse_condition(p);
  return s;
}

struct command parse_command(struct parser* p)
{
  struct command c;
  expect(p, LIT_STR);
  strcpy(c.name, p->last_token.identifier); 
  advance(p);
  c.arg_num = 0;
  while(p->last_token.type == LIT_NUM)
  {
    c.args[c.arg_num++] = p->last_token.value;
    advance(p);
  }

  return c;
}

void advance(struct parser* p)
{
  p->last_token = next_token(p->lexer);
}

struct rule parse_rule(struct parser *p)
{
  struct rule r;
 
  expect(p, KW_WHEN);
  advance(p);
  r.condition = parse_condition(p);
  expect(p, KW_ARROW);
  advance(p);
  printf("[DEBUG] type : %d\n", p->last_token.type);
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

void print_rule(struct rule r)
{
  printf("When %s (%d %d) then ", r.condition.name, r.condition.edge, r.condition.inverted);

  if(r.is_setter)
    printf("set %s to %s (%d, %d)\n", 
           r.setter.dest, r.setter.value.name, r.setter.value.edge, r.setter.value.inverted);
  else
    printf(" command %s with %d args\n", r.command.name, r.command.arg_num);

}
