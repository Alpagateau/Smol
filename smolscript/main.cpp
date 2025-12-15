#include <iostream>
#include "binaries.hpp"
#include "compiler.hpp"

int main(int argc, char** argv)
{
  if(argc == 1)
  {
    std::cout << "Usage : " << argv[0] << " <files>" << std::endl;
  }
  else
  {
    std::vector<program_t> programs;
    for(int i = 1; i < argc; i++)
    {
      programs.push_back(compile_file(argv[i]));
    }
    std::cout << "Got : " << programs.size() << " agents" << std::endl;
  } 

  return 0;
}
