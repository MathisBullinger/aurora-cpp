#pragma once

#include <string>
#include <map>
#include "util/gl.hpp"

namespace aur {

class Texture {
public:
  Texture(int type = GL_TEXTURE_2D);
  virtual ~Texture();
  
  void bind(unsigned int location = 0) const;
  void unbind() const;

  static void deleteTextures();

  template <typename T>
  static Texture* get(const std::string& path) {
    if (!textures.contains(path)) {
      textures.insert({ path, new T(path) });
      textures.at(path)->load();
    }
    return textures.at(path);
  }

  virtual void load() = 0;

protected:
  void load2D(const std::string& path, int type);

  const int type_;
  unsigned int id_ = 0;

private:
  static std::map<std::string, Texture*> textures;
};

class Texture2D : public Texture {
public:
  Texture2D(const std::string& path);
  void load();
  
private:
  const std::string path_;
};

}
