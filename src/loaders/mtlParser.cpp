#include "./mtlParser.hpp"

#include <iostream>
#include <fstream>
#include <exception>
#include "util/string.hpp"

namespace aur::loader::mesh {

std::map<std::string, Material*> parseMTL(const std::string& path) {

  std::ifstream file(path);
  if (!file.is_open()) 
    throw std::runtime_error(std::string("couldn't open material file: ") + path);
  
  Material* material = nullptr;
  std::map<std::string, Material*> materials;

  for (std::string line; std::getline(file, line);) {
    line = str::trim(line);
    if (!line.size() || line[0] == '#') continue;

    auto parts = str::split(line, ' ');

    if (parts[0] == "newmtl") {
      material = Material::get(path + parts[1]);
      materials.insert({ parts[1], material });
      continue;
    }

    if (!material) continue;

    Vector<3, float> cl;
    if (parts[0].starts_with("K"))
      for (unsigned int i = 0; i < 3; i++) 
        cl[i] = std::stod(parts[i + 1]);

    if (parts[0] == "Ka") material->ambient = cl;
    if (parts[0] == "Kd") material->diffuse = cl;
    if (parts[0] == "Ks") material->specular = cl;
    if (parts[0] == "Ns") material->specExp = std::stod(parts[1]);
  }
  
  return materials;
}

}
