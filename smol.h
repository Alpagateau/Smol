#ifndef SMOL_H
#define SMOL_H

#ifndef STR_LEN
#define STR_LEN 255
#endif

#ifndef MAX_ARG_NUM
#define MAX_ARG_NUM 10
#endif

#ifndef MAX_RULE_NUM
#define MAX_RULE_NUM 255
#endif

#ifndef MAX_IMG_NUM
#define MAX_IMG_NUM 16
#endif

#include <stdbool.h>

struct condition
{
  char name[STR_LEN];
  bool edge;
  bool inverted;
  bool constant;
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

struct image
{
  char name[STR_LEN];
  char pxl[16*16];
};

struct agent
{
  int gfx_index; 
  short x;
  short y;
};

struct program
{
  int img_nb;
  int rule_nb;
  int agent_nb;
  struct image images[MAX_IMG_NUM];
  struct rule rules[MAX_RULE_NUM];
};

#endif
