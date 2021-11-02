#pragma once

#include <string>
#include <vector>

namespace aur::loader {

class Mesh {
  public:
    bool loadOBJ(
      const std::string& path,
      std::vector<float>& vertices,
      std::vector<float>& uvs,
      std::vector<float>& normals
    );
};

}
