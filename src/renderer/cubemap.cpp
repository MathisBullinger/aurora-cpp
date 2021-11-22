#include "./cubemap.hpp"
#include "loaders/texture.hpp"

namespace aur {

Cubemap::Cubemap(const std::string& pathPrefix, const std::array<std::string, 6>& faces) 
  : Texture{GL_TEXTURE_CUBE_MAP}, 
    pathPrefix_{pathPrefix}, 
    faces_{faces} 
{};

void Cubemap::load() {
  GLC(glGenTextures(1, &id_));
  GLC(glBindTexture(GL_TEXTURE_CUBE_MAP, id_));

  for (unsigned int i = 0; i < faces_.size(); i++)
    load2D(pathPrefix_ + faces_[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);

  GLC(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLC(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLC(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLC(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLC(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

}
