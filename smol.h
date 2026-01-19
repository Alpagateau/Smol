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

#ifndef MAX_AGENTS_NUM
#define MAX_AGENTS_NUM 16
#endif

#ifndef MAX_FLAG_NUM
#define MAX_FLAG_NUM 16
#endif

#include <stdbool.h>

enum argument_type 
{
  ARG_NONE = 0,
  ARG_STR,
  ARG_INT,
  ARG_IMG,
  ARG_SPR
};

#define NEGATED 1
#define INSTANT 2

struct condition
{
  char name[STR_LEN];
  bool edge;
  bool inverted;
  bool constant;
};

struct argument
{
  enum argument_type agr_type;
  union {
    int value;
    char name[STR_LEN];
  };
};

struct command
{
  char name[STR_LEN];
  int arg_num; 
  struct argument args[MAX_ARG_NUM];
};

struct setter
{
  char dest[STR_LEN];
  struct condition value;
};

struct definer
{
  char name[STR_LEN];
  int value;
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
  char name[STR_LEN];
  int gfx_index; 
  short x;
  short y;
  bool visible;
};

struct program
{
  int img_nb;
  int rule_nb;
  int agent_nb;
  char agents[MAX_AGENTS_NUM][STR_LEN];
  struct image images[MAX_IMG_NUM];
  struct rule rules[MAX_RULE_NUM];
};

struct smol_rule
{
  char flags;
  int condition;
  int func;
  int arg_nb;
  int args[MAX_ARG_NUM];
};

struct smol_program
{
  int sprite_nb;
  int img_nb; 
  char images[MAX_IMG_NUM][16*16];
  int flg_nb;
  char flags[MAX_RULE_NUM]; 
  struct smol_rule rules[MAX_RULE_NUM];
  int rules_nb;
};

#endif
