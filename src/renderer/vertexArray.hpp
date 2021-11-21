#pragma once

#include "./vertexBuffer.hpp"
#include <vector>

namespace aur {

struct LayoutElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int sizeOfType(unsigned int type);
};

class BufferLayout {
public:
  template <typename T>
  void push(unsigned int count);

  const std::vector<LayoutElement>& getElements() const;
  unsigned int getStride() const;

private:
  std::vector<LayoutElement> elements_;
  unsigned int stride_ = 0;
};

class VertexArray {
public:
  VertexArray();
  ~VertexArray();

  void addBuffer(const VertexBuffer& vb, const BufferLayout& layout);
  void bind() const;
  void unbind() const;

private:
  unsigned int id_;
};
  
}
