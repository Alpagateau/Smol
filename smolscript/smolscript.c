#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include <string.h>

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

  char new_name[STR_LEN] = {};
  strcpy(new_name, argv[1]);
  char* dot = strrchr(new_name, '.');
  if(!dot)
    dot = new_name + strlen(new_name);

  strcpy(dot, ".rsmol");
  FILE* output = fopen(new_name, "wb");
  
  fwrite(&p, sizeof(struct program), 1, output);

  fclose(lexer.src); 
  fclose(output);
  return 0;
}
