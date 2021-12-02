#include "./material.hpp"

namespace aur {

std::map<std::string, Material*> Material::lib;

Material* Material::get(const std::string id) {
  if (!lib.contains(id)) lib.insert({ id, new Material{ id } });
  return lib.at(id);
}

void Material::deleteMaterials() {
  for (auto& [_, mat] : lib) 
    delete mat;
  lib.clear();  
}

}
