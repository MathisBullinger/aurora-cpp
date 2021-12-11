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
  static Texture* get(const std::string& path, int format = GL_SRGB) {
    if (!textures.contains(path)) {
      textures.insert({ path, new T(path) });
      textures.at(path)->load(format);
    }
    return textures.at(path);
  }

  static void add(const std::string& key, Texture* texture);

  virtual void load(int format) = 0;

protected:
  void load2D(const std::string& path, int type, int format);

  const int type_;
  unsigned int id_ = 0;

private:
  static std::map<std::string, Texture*> textures;
};

class Texture2D : public Texture {
public:
  Texture2D(const std::string& path);
  Texture2D(unsigned int width, unsigned int height, const void* data);
  void load(int format);
  
private:
  const std::string path_;
};

}
