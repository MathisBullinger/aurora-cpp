#include "./texture.hpp"
#include <iostream>
#include "util/path.hpp"

namespace aur::loader {

int Texture::loadBMP(const std::string& path, void** data, unsigned int* width, unsigned int* height) {
  auto file = fopen(path::join(path::TEXTURES, path).c_str(), "rb");
  if (!file) {
    std::cerr << "couldn't open file: " << path << std::endl;
    return 0;
  }
  unsigned char header[54];
  if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M') {
    std::cerr << "invalid BMP file: " << path << std::endl;
    return 0;
  }

  enum Compression {
    BI_RGB,
    BI_RLE8
  };

  *width                  = *(int*)&(header[0x12]);
  *height                 = *(int*)&(header[0x16]);
  const auto imageSize    = *(int*)&(header[0x02]) ?: *width * *height * 3;
  const auto dataPos      = *(int*)&(header[0x0A]);
  const auto compression  = *(int*)&(header[0x1E]);
  const auto channels     = *(short*)&(header[0x1C]) / 8;

  assert(channels == 3 || channels == 1);
  assert(compression == BI_RGB || compression == BI_RLE8);

  *data = new char [imageSize];

  fseek(file, dataPos, SEEK_CUR);
  fread(*data, 1, imageSize, file);
  fclose(file);

  if (compression != BI_RLE8) return imageSize;

  auto org = (char*)*data;
  *data = new char[*width * *height * 3];

  unsigned int pxI = 0;
  for (unsigned int i = 0; i < imageSize; i += 2) {
    if (org[i] == 0) {
      if (org[i+1] == 0) continue;
      if (org[i+1] == 1) break;
      assert(false);
    }
      
    for (unsigned int e = 0; e < org[i] * (3 / channels); e++)
      ((char*)*data)[pxI++] = org[i + 1];

  }

  delete org;
  return *width * *height * 3;
}
  
}
