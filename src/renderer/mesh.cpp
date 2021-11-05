#include "./mesh.hpp"
#include "loaders/mesh.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"
#include <vector>

#include <iostream>

namespace aur {

Mesh::Mesh(const std::string& path) {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  loader::mesh::loadOBJ(path, vertices, indices);

  for (auto i = 0; i < vertices.size(); i+= 3) {
    std::cout << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << std::endl;
  }

  vertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));
  indexBuffer = new IndexBuffer(&indices[0], indices.size());

  GLC(glEnableVertexAttribArray(0));
	GLC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

  // GLC(glGenBuffers(1, &nbo));
  // GLC(glBindBuffer(GL_ARRAY_BUFFER, nbo));
  // GLC(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW));

  // GLC(glGenBuffers(1, &ebo2));
  // GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2));
  // GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, normalInds.size() * sizeof(unsigned int), &normalInds[0], GL_STATIC_DRAW));

  // GLC(glEnableVertexAttribArray(1));
	// GLC(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
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
