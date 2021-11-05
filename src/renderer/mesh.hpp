#pragma once

#include <string>
#include <vector>
#include "math/vector.hpp"

namespace aur {

class Mesh {
public:
  Mesh(const std::string& path);
  int countVertices() const;
  int countIndices() const;

private:
  std::vector<float> vertices;
	std::vector<float> uvs;
	std::vector<float> normals;
	std::vector<unsigned int> indices;

  unsigned int vbo;
  unsigned int ebo;
};
  
}
