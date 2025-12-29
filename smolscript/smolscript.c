#include <ctype.h>
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

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

  while(parser.last_token.type != KW_EOF)
  {
    struct rule r = parse_rule(&parser);
    print_rule(r);
  }
  return 0;
}
