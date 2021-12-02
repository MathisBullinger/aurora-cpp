#include "./material.hpp"

namespace aur {

std::map<std::string, Material*> Material::lib;

Material* Material::get(const std::string id, const cl& ambient, const cl& diffuse, const cl& specular) {
  if (!lib.contains(id)) lib.insert({ id, new Material{ id, ambient, diffuse, specular } });
  return lib.at(id);
}

}
