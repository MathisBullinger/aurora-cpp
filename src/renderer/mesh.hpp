#pragma once

#include <string>
#include <map>
#include "renderer/vertexBuffer.hpp"
#include "renderer/indexBuffer.hpp"
#include "renderer/vertexArray.hpp"
#include "math/vector.hpp"
#include "renderer/material.hpp"

namespace aur {

class Mesh {
public:
  Mesh(const std::string& path, Vector<3, float> translate = {0,0,0});
  ~Mesh();
  
  const std::map<Material*, IndexBuffer*>& getMaterials() const;
  Vector<3, float> minPos() const;
  Vector<3, float> maxPos() const;

  void bind() const;
  void unbind() const;

  static Mesh* get(const std::string& path);
  static void deleteMeshes();

private:
  static std::map<std::string, Mesh*> instances;

  VertexArray vertexArray;
  VertexBuffer* vertexBuffer = nullptr;
  std::map<Material*, IndexBuffer*> mtlBuffers;
};
  
}
