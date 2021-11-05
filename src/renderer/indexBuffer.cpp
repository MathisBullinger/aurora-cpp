#include "./indexBuffer.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"

namespace aur {

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count): count{count} {
  GLC(glGenBuffers(1, &id));
  GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
  GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
  GLC(glDeleteBuffers(1, &id));
}

void IndexBuffer::bind() const {
  GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::unbind() const {
  GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

}
