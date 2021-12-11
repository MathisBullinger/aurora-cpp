#include "./texture.hpp"
#include "loaders/texture.hpp"

namespace aur {

std::map<std::string, Texture*> Texture::textures;

Texture::Texture(int type) : type_{type} {}

Texture::~Texture() {
  if (id_) GLC(glDeleteTextures(1, &id_));
}

void Texture::bind(unsigned int location) const {
  GLC(glActiveTexture(GL_TEXTURE0 + location));
  GLC(glBindTexture(type_, id_));
}

void Texture::unbind() const {
  GLC(glBindTexture(type_, 0));
}

void Texture::deleteTextures() {
  for (const auto& v : textures) delete v.second;
}

void Texture::load2D(const std::string& path, int type, int format) {
  void* data;
  unsigned int width, height;
  loader::bmp(path, &data, &width, &height);
  GLC(glTexImage2D(type, 0, format, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));
  delete [] (char*)data;
}

void Texture::add(const std::string& key, Texture* texture) {
  textures.insert({ key, texture });
}

Texture2D::Texture2D(const std::string& path) 
  : Texture{GL_TEXTURE_2D}, path_{path} {};

Texture2D::Texture2D(unsigned int width, unsigned int height, const void* data) {
  GLC(glGenTextures(1, &id_));
  GLC(glBindTexture(GL_TEXTURE_2D, id_));
  GLC(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));
}

void Texture2D::load(int format) {
  assert(!id_);
  GLC(glGenTextures(1, &id_));
  GLC(glBindTexture(GL_TEXTURE_2D, id_));
  load2D(path_, GL_TEXTURE_2D, format);
  GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
  GLC(glGenerateMipmap(GL_TEXTURE_2D));
}

}
