#include <stdio.h>
#include <raylib.h>
#include "smol.h"
#include "smollib/smollib.h"

int (*functions[20])(int args[], int arglen, struct agent sprites[], int sprlen);

int move(int args[], int len, struct agent sprites[], int sprlen)
{
  sprites[args[0]].x += args[1];
  sprites[args[0]].y += args[2];
}

int main(int argc, char** argv)
{
  functions[0] = move;

  if(argc == 1)
  {
    printf("Usage : %s <file>\n", argv[0]);
    return 1;
  }

  InitWindow(128, 128, "TEST");
  SetTargetFPS(3);
  Color palette[] = {
    WHITE,
    GRAY,
    BLACK,
    RED,
    GREEN,
    BLUE
  };

  struct agent agents[MAX_AGENTS_NUM] = {};
  char flags[MAX_FLAG_NUM] = {};
  flags[0] = 1; 

  struct smol_program prg;
  prg = load_program(argv[1]);

  printf("[DEBUG] Number of rules : %d\n", prg.rules_nb);
  printf("[DEBUG] First rule's condition : %d\n", prg.rules[0].condition);

  while(!WindowShouldClose())
  {
  BeginDrawing(); 
    ClearBackground(WHITE);
    DrawCircle(agents[0].x, agents[0].y, 3, RED);
    for(int i = 0; i < prg.rules_nb; i++) 
    {
      if( flags[prg.rules[i].condition] )
      {
        printf("Performing %d with first arg : %d\n", prg.rules[i].func, prg.rules[i].args[0]);
        functions[prg.rules[i].func](prg.rules[i].args, MAX_ARG_NUM, agents, MAX_AGENTS_NUM);
      }
    }
  EndDrawing();
  }
  CloseWindow();
  return 0;
}
