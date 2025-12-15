#ifndef SMOL_BINARIES_HPP
#define SMOL_BINARIES_HPP

#include "compiler.hpp"
#include <array>

struct Color
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};

#define WHITE (Color){0xFF, 0xFF, 0xFF, 0xFF}
#define BLACK (Color){0x00, 0x00, 0x00, 0xFF}
#define RED (Color){0xFF, 0x00, 0x00, 0xFF}
#define GREEN (Color){0x00, 0xFF, 0x00, 0xFF}
#define BLUE (Color){0x00, 0x00, 0xFF, 0xFF}
#define EMPTY (Color){0x00, 0x00, 0x00, 0x00}

constexpr std::array<Color, 8> PALETTE = 
{
  EMPTY,
  WHITE,
  BLACK,
  RED,
  GREEN,
  BLUE,
  (Color){0x99, 0x99, 0x99, 0xFF},
  (Color){0xFF, 0xFF, 0x00, 0xFF}
};

std::vector<Color> load_image(std::string path);
std::vector<unsigned char> rle_image(std::vector<Color> colors);
const Color &findClosestColor(const Color &pixel,unsigned char *out);

#endif
