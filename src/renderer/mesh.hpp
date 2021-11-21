#pragma once

#include <string>
#include <iterator>
#include "renderer/vertexBuffer.hpp"
#include "renderer/indexBuffer.hpp"
#include "renderer/vertexArray.hpp"
#include "math/vector.hpp"

namespace aur {

class Mesh {
public:
  Mesh(const std::string& path, Vector<3, float> translate = {0,0,0});
  ~Mesh();
  
  int countIndices() const;
  Vector<3, float> minPos() const;
  Vector<3, float> maxPos() const;

  void bind() const;
  void unbind() const;

private:
  VertexArray vertexArray;
  VertexBuffer* vertexBuffer = nullptr;
  IndexBuffer* indexBuffer = nullptr;
};
  
}
