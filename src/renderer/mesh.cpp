#include "./mesh.hpp"
#include "loaders/mesh.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"

namespace aur {

Mesh::Mesh(const std::string& path) {
  loader::mesh::loadOBJ(path, vertices, uvs, normals);

  GLC(glGenBuffers(1, &vbo));
  GLC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
  GLC(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW));
}

int Mesh::numVertices() const {
  return vertices.size();
}
  
}
