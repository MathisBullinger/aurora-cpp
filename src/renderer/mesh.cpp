#include "./mesh.hpp"
#include "loaders/mesh.hpp"
#include "loaders/objParser.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"
#include <vector>

namespace aur {

Mesh::Mesh(const std::string& path, Vector<3, float> translate) {
  std::vector<float> vertices;
  loader::mesh::MTLMap materials;
  loader::mesh::loadOBJ(path, vertices, materials);

  if (translate[0] != 0 || translate[1] != 0 || translate[2] != 0)
    for (unsigned int i = 0; i < vertices.size(); i+=8)
      for (unsigned int e = 0; e < 3; e++)
        vertices[i+e] += translate[e];


  vertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float), true);

  BufferLayout layout;
  layout.push<float>(3);
  layout.push<float>(3);
  layout.push<float>(2);

  vertexArray.addBuffer(*vertexBuffer, layout);

  for (auto& [material, indices] : materials)
    mtlBuffers.insert({ material, new IndexBuffer(&indices[0], indices.size()) });
}

Mesh::~Mesh() {
  delete vertexBuffer;
  for (auto& [m, indBuf] : mtlBuffers) delete indBuf;
  mtlBuffers.clear();
}

void Mesh::bind() const {
  vertexArray.bind();
}

void Mesh::unbind() const {
  vertexArray.unbind();
}

const std::map<Material*, IndexBuffer*>& Mesh::getMaterials() const {
  return mtlBuffers;
}

Vector<3, float> Mesh::minPos() const {
  Vector<3, float> min{1e9, 1e9, 1e9};
  auto size = vertexBuffer->getSize() / sizeof(float);
  auto data = (float*)vertexBuffer->getData();

  for (unsigned int i = 0; i < size; i += 8) {
    for (int e = 0; e < 3; e++) {
      if (data[i+e] < min[e]) min[e] = data[i+e];
    }
  }

  return min;
}

Vector<3, float> Mesh::maxPos() const {
  Vector<3, float> max{-1e9, -1e9, -1e9};
  auto size = vertexBuffer->getSize() / sizeof(float);
  auto data = (float*)vertexBuffer->getData();

  for (unsigned int i = 0; i < size; i += 8) {
    for (int e = 0; e < 3; e++) {
      if (data[i+e] > max[e]) max[e] = data[i+e];
    }
  }

  return max;
}
  
}
