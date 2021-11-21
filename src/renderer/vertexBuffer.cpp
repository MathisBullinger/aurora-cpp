#include "./vertexBuffer.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"

namespace aur {

VertexBuffer::VertexBuffer(void* data, unsigned int size, bool store): size_{size} {
  GLC(glGenBuffers(1, &id_));
  GLC(glBindBuffer(GL_ARRAY_BUFFER, id_));
  GLC(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  if (store) {
    data_ = malloc(size);
    memcpy(data_, data, size);
  }
}

VertexBuffer::~VertexBuffer() {
  GLC(glDeleteBuffers(1, &id_));
  free();
}

void VertexBuffer::bind() const {
  GLC(glBindBuffer(GL_ARRAY_BUFFER, id_));
}

void VertexBuffer::unbind() const {
  GLC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

const void* VertexBuffer::getData() const {
  assert(data_);
  return data_;
}

void VertexBuffer::free() {
  if (!data_) return;
  std::free(data_);
  data_ = nullptr;
}

int VertexBuffer::getSize() const {
  return size_;
}
  
}
