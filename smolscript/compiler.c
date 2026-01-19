#include "compiler.h"
#include <stdlib.h>
#include <string.h>

int number_of_args(struct smol_function f) {
  int i = 0;
  for (int s = 0; s < MAX_ARG_NUM; s++) {
    if (f.args[i] == ARG_NONE)
      break;
    i++;
  }
  return i;
}

int index_of(char *key, char haysatck[][STR_LEN], int len) {
  for (int i = 0; i < len; i++) {
    if (strcmp(key, haysatck[i]))
      return i;
  }
  return -1;
}

int index_of_func(char *key, struct smol_function *haysatck, int len) {
  for (int i = 0; i < len; i++) {
    if (strcmp(key, haysatck[i].name))
      return i;
  }
  return -1;
}

struct smol_rule compile_rule(
  struct rule r, 
  char flags[][STR_LEN], int *flags_num, 
  char images[][STR_LEN], int img_len,
  char sprites[][STR_LEN], int spr_len,
  struct smol_function *funcs, int func_num
  ) {
  struct smol_rule sr = {};
  sr.flags = 0;
  if (r.condition.inverted)
    sr.flags |= NEGATED;
  if (r.condition.edge)
    sr.flags |= INSTANT;

  int condition = index_of(r.condition.name, flags, *flags_num);
  if (condition < 0) {
    if (*flags_num >= MAX_FLAG_NUM) {
      printf("[ERROR] : Too many booleans (max : %d\n)", MAX_FLAG_NUM);
      exit(1);
    }
    strcpy(flags[*flags_num], r.condition.name);
    condition = *flags_num++;
  }
  sr.condition = condition;
  sr.func = index_of_func(r.command.name, funcs, func_num);
  if (sr.func < 0) {
    printf("[ERROR] : Unrecognized function : %s\n", r.command.name);
    exit(1);
  }
  if (r.command.arg_num != number_of_args(funcs[sr.func]))
  {
    printf("[ERROR] : Not the right number of arguments for %s : needed %d, got %d\n", 
           funcs[sr.func].name,
           number_of_args(funcs[sr.func]),
           r.command.arg_num
           );
    exit(1);
  }
  //Deport args
  for(int i = 0; i < r.command.arg_num; i++)
  {
    if(funcs[sr.func].args[i] == ARG_INT)
    {
      if(r.command.args[i].agr_type != ARG_INT)
      {
        printf("[ERROR] : Uncompatible types : needed int got %d\n", r.command.args[i].agr_type);
        exit(1);
      }
      sr.args[i] = r.command.args[i].value;
    }
    if(funcs[sr.func].args[i] == ARG_IMG)
    {
      if(r.command.args[i].agr_type != ARG_STR)
      {
        printf("[ERROR] : Uncompatible types : needed IMG got %d\n", r.command.args[i].agr_type);
        exit(1);
      }
      sr.args[i] = index_of(r.command.args[i].name, images, img_len); 
    }
    if(funcs[sr.func].args[i] == ARG_SPR)
    {
      if(r.command.args[i].agr_type != ARG_STR)
      {
        printf("[ERROR] : Uncompatible types : needed SPR got %d\n", r.command.args[i].agr_type);
        exit(1);
      }
      sr.args[i] = index_of(r.command.args[i].name, sprites, spr_len);
    }
  }

  return sr;
}

struct smol_program compile(struct program p, char **flags, int flags_num,
                            struct smol_function *funcs, int func_num) {
  struct smol_program sp = {};

  char image_table[MAX_IMG_NUM][STR_LEN];
  char sprite_table[MAX_AGENTS_NUM][STR_LEN];
  char total_flags[MAX_FLAG_NUM][STR_LEN];

  memcpy(total_flags, flags, sizeof(char[STR_LEN]) * flags_num);

  int flag_count = flags_num;

  // Copy images
  sp.img_nb = p.img_nb;
  for (int i = 0; i < p.img_nb; i++) {
    strcpy(image_table[i], p.images[i].name);
    memcpy(sp.images[i], p.images[i].pxl, sizeof(p.images[i].pxl));
  }

  // Prepare the sprite table
  sp.sprite_nb = p.agent_nb;
  for (int i = 0; i < sp.sprite_nb; i++) {
    strcpy(sprite_table[i], p.agents[i]);
  }

  // Prepare the smol rules
  for (int i = 0; i < p.rule_nb; i++) {
    sp.rules[sp.rules_nb++] =
        compile_rule(
          p.rules[i], 
          total_flags, &flag_count,
          image_table, sp.img_nb,
          sprite_table, sp.sprite_nb,
          funcs, func_num
        );
  }

  return sp;
}
