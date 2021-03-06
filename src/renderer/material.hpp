#pragma once

#include <string>
#include <map>
#include "math/vector.hpp" 
#include "./texture.hpp"

namespace aur {

using cl = Vector<3, float>;

struct Material {
  const std::string id;
  cl albedo = {1, 1, 1};
  Texture* texture = nullptr;
  Texture* normalMap = nullptr;
  float metallic = .01;
  float roughness = .5;
  Texture* roughnessMap = nullptr;
  float ao = 0.1;
  Texture* aoMap = nullptr;

  static std::map<std::string, Material*> lib;
  static Material* get(const std::string id);
  static void deleteMaterials();
};

}
