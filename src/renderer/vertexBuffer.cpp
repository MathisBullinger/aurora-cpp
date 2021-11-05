#include "./vertexBuffer.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"

namespace aur {

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
  GLC(glGenBuffers(1, &id));
  GLC(glBindBuffer(GL_ARRAY_BUFFER, id));
  GLC(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
  GLC(glDeleteBuffers(1, &id));
}

void VertexBuffer::bind() const {
  GLC(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void VertexBuffer::unbind() const {
  GLC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
  
}
