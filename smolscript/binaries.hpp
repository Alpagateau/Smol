#ifndef SMOL_BINARIES_HPP
#define SMOL_BINARIES_HPP

#include "compiler.hpp"
#include "format.hpp"

std::vector<unsigned char> compile_rules(program_t prgm);
std::vector<unsigned char> compiled_image(program_t prgm);
#endif
