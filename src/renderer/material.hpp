#pragma once

#include <string>
#include <map>
#include "math/vector.hpp"

namespace aur {

using cl = Vector<3, float>;

struct Material {
  const std::string id;
  cl ambient;
  cl diffuse;
  cl specular;

  static std::map<std::string, Material*> lib;
  static Material* get(const std::string id);
  static void deleteMaterials();
};

}
