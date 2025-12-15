#include <iostream>
#include "compiler.hpp"

int main(int argc, char** argv)
{
  if(argc == 1)
  {
    std::cout << "Usage : " << argv[0] << " <files>" << std::endl;
  }
  else
  {
    for(int i = 1; i < argc; i++)
    {
      compile_file(argv[i]);
    }
  }
  return 0;
}
