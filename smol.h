#ifndef SMOL_H
#define SMOL_H

#ifndef STR_LEN
#define STR_LEN 255
#endif

#ifndef MAX_ARG_NUM
#define MAX_ARG_NUM 10
#endif

#include <stdbool.h>

struct condition
{
  char name[STR_LEN];
  bool edge;
  bool inverted;
};

struct command
{
  char name[STR_LEN];
  int arg_num; 
  int args[MAX_ARG_NUM];
};

struct setter
{
  char dest[STR_LEN];
  struct condition value;
};

struct rule
{
  struct condition condition;
  bool is_setter;
  union{
    struct setter setter;
    struct command command;
  };
};

#endif
