#pragma once

#include <string>
#include <map>
#include "math/vector.hpp" 
#include "./texture.hpp"

namespace aur {

using cl = Vector<3, float>;

struct Material {
  const std::string id;
  cl ambient = {1, .5, .31};
  cl diffuse = {1, .5, .31};
  cl specular = {.5, .5, .5};
  float specExp = 32;
  Texture* texture = nullptr;

  static std::map<std::string, Material*> lib;
  static Material* get(const std::string id);
  static void deleteMaterials();
};

}
