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

extern std::vector<Color> palette;
extern std::vector<func> known_commands;

#endif
