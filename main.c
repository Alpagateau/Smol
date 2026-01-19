#include <stdio.h>
#include <raylib.h>
#include "smol.h"
#include "smollib/smollib.h"

int main(int argc, char** argv)
{
  if(argc == 1)
    return 1;

  InitWindow(128, 128, "TEST");

  Color palette[] = {
    WHITE,
    GRAY,
    BLACK,
    RED,
    GREEN,
    BLUE
  };

  struct program prg;
  prg = load_program(argv[1]);

  printf("[DEBUG] Number of rules : %d\n", prg.rule_nb);
  printf("[DEBUG] First rule's condition : %s", prg.rules[0].condition.name);

  Texture images[64] = {};
  for(int i = 0; i < prg.img_nb; i++)
  {
    Image img = GenImageColor(16, 16, WHITE);
    for(int j = 0; j < 16 * 16; j++)
    { 
      ImageDrawPixel(&img, j%16, (j - (j%16))/16, palette[prg.images[i].pxl[j]]);
    }
    images[i] = LoadTextureFromImage(img);
  }

  while(!WindowShouldClose())
  {
    BeginDrawing();
      ClearBackground(WHITE);
      for(int i = 0; i < prg.img_nb; i++)
      {
        DrawTextureEx(images[i],(Vector2){ 0, 0}, 0, 2, WHITE);
      }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
