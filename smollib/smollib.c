#include <stdio.h>
#include <stdlib.h>
#include "smollib.h"

struct program load_program(char* path)
{
  FILE* file = fopen(path, "rb");
  if(file == NULL)
    exit(1);
  struct program prgm; 
  fread(&prgm, sizeof(struct program), 1, file);
  return prgm;
}
