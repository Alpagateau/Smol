#include <stdio.h>
#include <stdlib.h>
#include "smollib.h"

struct smol_program load_program(char* path)
{
  FILE* file = fopen(path, "rb");
  if(file == NULL)
    exit(1);
  struct smol_program prgm; 
  fread(&prgm, sizeof(struct smol_program), 1, file);
  return prgm;
}
