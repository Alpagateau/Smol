#include "binaries.hpp"
#include "compiler.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cout << "Usage : " << argv[0] << " <files>" << std::endl;
  } else {
    std::vector<program_t> programs;
    for (int i = 1; i < argc; i++) {
      programs.push_back(compile_file(argv[i]));
    }
    // std::cout << "Got : " << programs.size() << " agents" << std::endl;
    for (int p = 0; p < programs.size(); p++) {
      std::vector<unsigned char> bytes = compile_rules(programs[p]);
      std::ofstream output("obj.obj");
      output.write((const char*)&bytes[0], bytes.size());
    }
  }

  return 0;
}
