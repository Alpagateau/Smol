#ifndef SMOL_FORMAT_HPP
#define SMOL_FORMAT_HPP

#include <string>
#include <vector>

struct Color
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};

struct func
{
  std::string ident;
  int arg_num;
};

struct object_t
{
  std::vector<unsigned char> image;
  std::vector<unsigned char> commands;
};

static std::vector<Color> palette = 
{
  {0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0x00, 0x00, 0xFF},
  {0x00, 0xFF, 0x00, 0xFF},
  {0x00, 0x00, 0xFF, 0xFF}
};

static std::vector<func> known_commands = 
{
  {"move", 2},
  {"exit", 0}
};

static std::vector<std::string> known_variables = 
{
  "always"
};

#endif
