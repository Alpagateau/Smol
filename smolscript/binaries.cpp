#include "binaries.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <limits>
#include <stdexcept>
#include <vector>

inline int colorDistanceSq(const Color &a, const Color &b) {
  int dr = int(a.r) - int(b.r);
  int dg = int(a.g) - int(b.g);
  int db = int(a.b) - int(b.b);
  int da = int(a.a) - int(b.a);
  return dr * dr + dg * dg + db * db + da * da;
}

// TODO Do a real algorithm
const Color &findClosestColor(const Color &pixel, unsigned char *out) {
  const Color *best = &PALETTE[0];
  int bestDist = std::numeric_limits<int>::max();
  for (unsigned char i = 0; i < 8; i++) {
    int d = colorDistanceSq(pixel, PALETTE[i]);
    if (d < bestDist) {
      bestDist = d;
      best = &PALETTE[i];
      if (out != NULL)
        *out = i;
    }
  }
  return *best;
}

std::vector<Color> load_image(std::string path) {
  int width, height, channel;
 
  const char* filename = path.c_str();
  unsigned char *pixels_arr = stbi_load(filename, &width, &height, &channel, 4);
  std::vector<unsigned char> pixels(pixels_arr, pixels_arr + (width * height * channel));
  
  if(width != 32 || height != 32)
    throw std::runtime_error("Images need to be 32x32 pixels");
  std::vector<Color> colors = {};
  for (unsigned int i = 0; i < pixels.size(); i += 4) {
    Color c = {pixels[i + 0], pixels[i + 1], pixels[i + 2], pixels[i + 3]};
    colors.push_back(c);
  }
  return colors;
}

std::vector<unsigned char> rle_image(std::vector<Color> colors) {
  std::vector<unsigned char> output = {};
  unsigned char rle_idx = 0;
  unsigned char current_idx = 0;
  unsigned char counter = 0;
  findClosestColor(colors[0], &rle_idx);
  current_idx = rle_idx;
  for (unsigned int i = 0; i < colors.size(); i++) {
    findClosestColor(colors[i], &current_idx);
    if (current_idx == rle_idx && counter < 255) {
      counter++;
    } else {
      output.push_back(counter);
      counter = 1;
      output.push_back(rle_idx); 
      rle_idx = current_idx;
    }
  } 
  output.push_back(counter);
  output.push_back(rle_idx); 
  return output;
}
