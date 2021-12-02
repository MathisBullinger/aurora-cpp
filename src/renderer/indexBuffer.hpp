#pragma once

namespace aur {

class IndexBuffer {
public:
  IndexBuffer(const unsigned int* data, const unsigned int count);
  ~IndexBuffer();
  
  void bind() const;
  void unbind() const;

  const unsigned int count;

private:
  unsigned int id;
};
  
}
