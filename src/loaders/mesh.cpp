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
  std::ifstream file(path::join(path::MESHES, path));
  if (!file.is_open()) {
    std::cerr << "couldn't open file: " << path << std::endl;
    return false;
  }
  OBJParser parser(path, file, vertices, mtlMap);
  parser.parse();
  file.close();
  return true;
}

}
