#include "binaries.hpp"
#include "compiler.hpp"
#include "format.hpp"
#include <climits>
#include <iostream>
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

std::vector<unsigned char> rle(std::vector<unsigned char> &input) {
  unsigned char counter = 0;
  unsigned char value = input[0];
  std::vector<unsigned char> output = {};
  for (int i = 0; i < input.size(); i++) {
    if (value != input[i] || counter == 255) {
      output.push_back(value);
      output.push_back(counter);
      value = input[i];
      counter = 1;
    } else {
      counter++;
    }
  }
  output.push_back(value);
  output.push_back(counter);
  return output;
}

unsigned char closest_color(Color c) {
  int min_dst = INT_MAX;
  int min_idx = 0;

  for (int i = 0; i < palette.size(); i++) {
    int dst = (c.a - palette[i].a) * (c.a - palette[i].a) * 100 +
              (c.r - palette[i].r) * (c.r - palette[i].r) +
              (c.g - palette[i].g) * (c.g - palette[i].g) +
              (c.b - palette[i].b) * (c.b - palette[i].b);
    if (dst < min_dst) {
      min_dst = dst;
      min_idx = i;
    }
  }
  return (unsigned char)min_idx;
}

std::vector<unsigned char> load_image_with_palette(std::string path) {
  int width, height, channels;
  std::cout << "Loading : " << path << std::endl;
  unsigned char *bytes = stbi_load(path.c_str(), &width, &height, &channels, 4);
  if (width != 32 || height != 32){
    std::cout << width << "x" << height << std::endl;
    throw std::runtime_error("Image must be a 32x32 png");
  }
  std::vector<unsigned char> output = {};
  for (int i = 0; i < width * height * 4; i += 4) {
    Color c = {.r = bytes[i + 0],
               .g = bytes[i + 1],
               .b = bytes[i + 2],
               .a = bytes[i + 3]};
    output.push_back(closest_color(c));
  }
  return output;
}

std::vector<unsigned char> compiled_image(program_t prgm) {
  std::vector<unsigned char> compiled = {};
  std::vector<unsigned char> img = load_image_with_palette(prgm.load.path);
  compiled = rle(img);
  return compiled;
}

std::vector<unsigned char> compile_rules(program_t prgm) {
  std::vector<unsigned char> compiled = {};

  for (int i = 0; i < prgm.rules.size(); i++) {
    rule_t current_rule = prgm.rules[i];
    unsigned char flags = 0;
    unsigned char cond_code = 0;
    flags = (current_rule.condition.negated ? 1 : 0) |
            (current_rule.condition.rising_edge ? 2 : 0);
    int idx = -1;
    for (int j = 0; j < known_variables.size(); j++) {
      if (known_variables[j] == current_rule.condition.name) {
        idx = j;
        break;
      }
    }
    if (idx < 0) {
      throw std::runtime_error("unknown variable");
    }
    cond_code = (unsigned char)idx;
    compiled.push_back(flags);
    compiled.push_back(cond_code);

    // Now the command
    idx = -1;
    unsigned char com_code = 0;
    for (int j = 0; j < known_commands.size(); j++) {
      if (known_commands[j].ident == current_rule.command.name) {
        idx = j;
        break;
      }
    }

    if (idx < 0)
      throw std::runtime_error("Unknown command");
    if (known_commands[idx].arg_num != current_rule.command.args.size())
      throw std::runtime_error("Incorrect number of args");

    com_code = (unsigned char)idx;
    compiled.push_back(com_code);
    for (int j = 0; j < current_rule.command.args.size(); j++) {
      unsigned int *uvalue = (unsigned int *)&current_rule.command.args[j];
      unsigned int new_value = *uvalue;
      compiled.push_back((new_value & 0xFF000000u) >> 24);
      compiled.push_back((new_value & 0x00FF0000u) >> 16);
      compiled.push_back((new_value & 0x0000FF00u) >> 8);
      compiled.push_back((new_value & 0x000000FFu) >> 0);
    }
  }
  return compiled;
}
