#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include <string.h>

#define DEBUG

int main(int argc, char **argv)
{
  if(argc < 2) 
  {
    printf("Usage : %s <files>\n", argv[0]);
    return 0;
  }

  struct lexer lexer;
  struct parser parser;
  init_lexer(&lexer, fopen(argv[1], "r"));
  init_parser(&parser, &lexer);
  
  if(lexer.src == NULL)
    return 1;

  struct program p = parse_program(&parser);
  
  char flags[MAX_FLAG_NUM][STR_LEN] = {"always", "A", "B", "C"};
  struct smol_function sf[] = {
    {
      .name = "move", 
      .args = {ARG_INT, ARG_INT, ARG_NONE}
    },
  };
  struct smol_program sp = compile(p, flags, 4, sf, 1);

  char new_name[STR_LEN] = {0};
  strcpy(new_name, argv[1]);
  char* dot = strrchr(new_name, '.');
  if(!dot)
    dot = new_name + strlen(new_name);

  strcpy(dot, ".smolo");
  FILE* output = fopen(new_name, "wb");
  
  fwrite(&sp, sizeof(struct smol_program), 1, output);

  fclose(lexer.src); 
  fclose(output);
  return 0;
}
