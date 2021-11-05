#pragma once

#include <string>
#include <vector>

namespace aur::loader::mesh {

bool loadOBJ(
  const std::string& path,
  std::vector<float>& vertices,
  std::vector<unsigned int>& indices
);

}
