#include "./mesh.hpp"
#include "loaders/mesh.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"
#include <vector>

namespace aur {

struct Vertex {
  float position[3];
  float normal[3];
  float uv[2];
};

Mesh::Mesh(const std::string& path) {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  loader::mesh::loadOBJ(path, vertices, indices);

  vertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));
  indexBuffer = new IndexBuffer(&indices[0], indices.size());

  GLC(glEnableVertexAttribArray(0));
  GLC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)));

  GLC(glEnableVertexAttribArray(1));
  GLC(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

  GLC(glEnableVertexAttribArray(2));
  GLC(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)));
}

Mesh::~Mesh() {
  delete vertexBuffer;
  delete indexBuffer;
}

int Mesh::countIndices() const {
  if (!indexBuffer) return 0;
  return indexBuffer->count;
}
  
}
