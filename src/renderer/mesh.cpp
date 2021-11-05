#include "./mesh.hpp"
#include "loaders/mesh.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"

namespace aur {

Mesh::Mesh(const std::string& path) {
  loader::mesh::loadOBJ(path, vertices, uvs, normals, indices);

  GLC(glGenBuffers(1, &vbo));
  GLC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  GLC(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW));

  GLC(glGenBuffers(1, &ebo));
  GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
  GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

}

int Mesh::countVertices() const {
  return vertices.size();
}

int Mesh::countIndices() const {
  return indices.size();
}
  
}
