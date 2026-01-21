#include <stdio.h>
#include <raylib.h>
#include "smol.h"
#include "smollib/smollib.h"

int main(int argc, char** argv)
{
  if(argc == 1)
  {
    printf("Usage : %s <file>\n", argv[0]);
    return 1;
  }

  InitWindow(128, 128, "TEST");

  Color palette[] = {
    WHITE,
    GRAY,
    BLACK,
    RED,
    GREEN,
    BLUE
  };

  struct smol_program prg;
  prg = load_program(argv[1]);

  printf("[DEBUG] Number of rules : %d\n", prg.rules_nb);
  printf("[DEBUG] First rule's condition : %d\n", prg.rules[0].condition);

  while(!WindowShouldClose())
  {
    BeginDrawing();
      ClearBackground(WHITE);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
