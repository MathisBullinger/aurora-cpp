#include "./vertexArray.hpp"
#include <GL/glew.h>
#include "util/gl.hpp"

namespace aur {

unsigned int LayoutElement::sizeOfType(unsigned int type) {
  switch (type) {
    case GL_FLOAT:         return sizeof(GLfloat);
    case GL_UNSIGNED_INT:  return sizeof(GLuint);
  }
  assert(false);
}

template <>
void BufferLayout::push<float>(unsigned int count) {
  elements_.push_back({ GL_FLOAT, count, GL_FALSE });
  stride_ += count * LayoutElement::sizeOfType(GL_FLOAT);
}

template <>
void BufferLayout::push<unsigned int>(unsigned int count) {
  elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
  stride_ += count * LayoutElement::sizeOfType(GL_UNSIGNED_INT);
}

const std::vector<LayoutElement>& BufferLayout::getElements() const {
  return elements_;
}

unsigned int BufferLayout::getStride() const {
  return stride_; 
}

VertexArray::VertexArray() {
  GLC(glGenVertexArrays(1, &id_));
}

VertexArray::~VertexArray() {
  GLC(glDeleteVertexArrays(1, &id_));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const BufferLayout& layout) {
  bind(); 
  vb.bind();
  const auto& elements = layout.getElements();
  unsigned int offset = 0;

  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& [type, count, normalized] = elements[i];
    GLC(glEnableVertexAttribArray(i));
    GLC(glVertexAttribPointer(i, count, type, normalized, layout.getStride(), (void*)offset));
    offset += count * LayoutElement::sizeOfType(type);
  }
}

void VertexArray::bind() const {
  GLC(glBindVertexArray(id_));
}

void VertexArray::unbind() const {
  GLC(glBindVertexArray(0));
}

}
