#ifndef SMOL_COMPILER_H
#define SMOL_COMPILER_H

#include "parser.h"

struct smol_function
{
  char name[STR_LEN];
  enum argument_type args[MAX_ARG_NUM];
};

struct smol_program compile(
  struct program p, 
  char** flags, 
  int flags_num,
  struct smol_function* funcs,
  int func_num
  );

int index_of(char* key, char haysatck[][STR_LEN], int len);

#endif
