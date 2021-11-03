#pragma once

#include <string>
#include <vector>
#include "math/vector.hpp"

namespace aur {

class Mesh {
public:
  Mesh(const std::string& path);
  int numVertices() const;

private:
  std::vector<float> vertices;
	std::vector<float> uvs;
	std::vector<float> normals;

  unsigned int vbo;
};
  
}
