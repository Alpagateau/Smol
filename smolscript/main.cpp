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
    for(int i = 1; i < argc; i++)
    {
      compile_file(argv[i]);
    }
  }
  
  std::vector<Color> pixels = load_image("test.png");
  std::vector<unsigned char> rle = rle_image(pixels);

  for(int i = 0; i < rle.size(); i+=2)
  {
      std::cout << "Number : " << (int)rle[i] << " | Index : " << (int)rle[i+1] << std::endl;
  }

  return 0;
}
