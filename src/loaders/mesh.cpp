#include "./mesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <array>
#include "./objParser.hpp"
#include "util/path.hpp"

namespace aur::loader::mesh {

bool loadOBJ(const std::string& path, std::vector<float>& vertices, std::vector<unsigned int>& indices) {
  std::ifstream file(path::join(path::MESHES, path));
  if (!file.is_open()) {
    std::cerr << "couldn't open file: " << path << std::endl;
    return false;
  }
  OBJParser parser(file, vertices, indices);
  parser.parse();
  file.close();
  return true;
}

}
