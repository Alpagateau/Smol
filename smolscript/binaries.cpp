#include "binaries.hpp"
#include "format.hpp"
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>

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

    if(idx < 0)
      throw std::runtime_error("Unknown command");
    if(known_commands[idx].arg_num != current_rule.command.args.size())
      throw std::runtime_error("Incorrect number of args");

    com_code = (unsigned char)idx;    
  }

  return compiled;
}
