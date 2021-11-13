#include "./texture.hpp"
#include "util/gl.hpp"
#include <iostream>
#include <GL/glew.h>

namespace aur::loader {

int Texture::loadBMP(const std::string& path) {
  auto file = fopen(path.c_str(), "rb");
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

  const auto width        = *(int*)&(header[0x12]);
  const auto height       = *(int*)&(header[0x16]);
  const auto imageSize    = *(int*)&(header[0x02]) ?: width * height * 3;
  const auto dataPos      = *(int*)&(header[0x0A]);
  const auto compression  = *(int*)&(header[0x1E]);
  const auto channels     = *(short*)&(header[0x1C]) / 8;

  assert(channels == 3 || channels == 1);
  assert(compression == BI_RGB || compression == BI_RLE8);

  auto data = new unsigned char [imageSize];
  unsigned char* pxData = data;

  fseek(file, dataPos, SEEK_CUR);
  fread(data, 1, imageSize, file);
  fclose(file);

  if (compression == BI_RLE8) {
    pxData = new unsigned char[width * height * 3];

    unsigned int pxI = 0;
    for (unsigned int i = 0; i < imageSize; i += 2) {
      if (data[i] == 0) {
        if (data[i+1] == 0) continue;
        if (data[i+1] == 1) break;
        assert(false);
      }
      
      for (unsigned int e = 0; e < data[i] * (3 / channels); e++)
        pxData[pxI++] = data[i + 1];
    }
  }

  unsigned int textureId;
  GLC(glGenTextures(1, &textureId));
  GLC(glBindTexture(GL_TEXTURE_2D, textureId));
  GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pxData));
  GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLC(glGenerateMipmap(GL_TEXTURE_2D));

  return textureId;
}
  
}
