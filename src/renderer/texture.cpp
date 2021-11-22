#include "./texture.hpp"
#include "loaders/texture.hpp"
#include "util/gl.hpp"

namespace aur {

std::map<std::string, Texture*> Texture::textures;

Texture::Texture(const std::string& path) {
  loader::Texture textureLoader;
  void* data;
  unsigned int width, height;
  textureLoader.loadBMP(path, &data, &width, &height);

  GLC(glGenTextures(1, &id_));
  GLC(glBindTexture(GL_TEXTURE_2D, id_));
  GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));
  GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLC(glGenerateMipmap(GL_TEXTURE_2D));

  delete [] (char*)data;
}

Texture::~Texture() {
  GLC(glDeleteTextures(1, &id_));
}

void Texture::bind() const {
  GLC(glBindTexture(GL_TEXTURE_2D, id_));
}

void Texture::unbind() const {
  GLC(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture* Texture::get(const std::string& path) {
  if (!textures.contains(path)) textures.insert({ path, new Texture(path) });
  return textures.at(path);
}

void Texture::deleteTextures() {
  for (const auto& v : textures) delete v.second;
}

}
