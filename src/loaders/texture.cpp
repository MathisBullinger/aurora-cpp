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

  unsigned int dataPos    = *(int*)&(header[0x0A]);
  unsigned int imageSize  = *(int*)&(header[0x22]);
  unsigned int width      = *(int*)&(header[0x12]);
  unsigned int height     = *(int*)&(header[0x16]);

  if (!imageSize) imageSize = width * height * 3;
  if (!dataPos) dataPos = 54;

  auto data = new unsigned char [imageSize];
  fread(data, 1, imageSize, file);
  fclose(file);

  unsigned int textureId;
  GLC(glGenTextures(1, &textureId));
  GLC(glBindTexture(GL_TEXTURE_2D, textureId));
  GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));
  GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLC(glGenerateMipmap(GL_TEXTURE_2D));

  return textureId;
}
  
}
