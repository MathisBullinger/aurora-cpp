#pragma once

#include <string>
#include "renderer/vertexBuffer.hpp"
#include "renderer/indexBuffer.hpp"

namespace aur {

class Mesh {
public:
  Mesh(const std::string& path);
  ~Mesh();
  
  int countIndices() const;

private:
  unsigned int vbo;
  unsigned int ebo;
  unsigned int nbo;
  unsigned int ebo2;

  VertexBuffer* vertexBuffer = nullptr;
  IndexBuffer* indexBuffer = nullptr;
};
  
}
