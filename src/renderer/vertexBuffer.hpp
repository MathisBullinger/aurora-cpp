#pragma once

namespace aur {

class VertexBuffer {
public:
  VertexBuffer(void* data, unsigned int size, bool store = false);
  ~VertexBuffer();
  
  void bind() const;
  void unbind() const;

  const void* getData() const;
  void free();
  int getSize() const;

private:
  unsigned int id_;
  unsigned int size_;
  void* data_ = nullptr;
};
  
}
