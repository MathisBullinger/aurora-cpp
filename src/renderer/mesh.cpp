#include "./mesh.hpp"
#include "loaders/mesh.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"
#include <vector>

namespace aur {

Mesh::Mesh(const std::string& path) {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  loader::mesh::loadOBJ(path, vertices, indices);

  vertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));
  indexBuffer = new IndexBuffer(&indices[0], indices.size());

  GLC(glEnableVertexAttribArray(0));
	GLC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0));

  GLC(glEnableVertexAttribArray(1));
	GLC(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
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
