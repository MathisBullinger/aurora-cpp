#pragma once

#include <string>
#include <map>

namespace aur {

class Texture {
public:
  Texture(const std::string& path);
  ~Texture();
  
  void bind() const;
  void unbind() const;

  static Texture* get(const std::string& path);
  static void deleteTextures();

private:
  unsigned int id_;
  static std::map<std::string, Texture*> textures;
};

}
