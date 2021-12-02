#include "./mesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <array>
#include "util/path.hpp"

namespace aur::loader::mesh {

bool loadOBJ(const std::string& path, std::vector<float>& vertices, MTLMap& mtlMap) {
  auto comPath = path::join(path::MESHES, path);
  std::ifstream file(comPath);
  if (!file.is_open()) {
    std::cerr << "couldn't open file: " << path << std::endl;
    return false;
  }
  OBJParser parser(comPath, file, vertices, mtlMap);
  parser.parse();
  file.close();
  return true;
}

}
