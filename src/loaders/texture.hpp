#pragma once

#include <string>

namespace aur::loader {

class Texture {
  public:
    int loadBMP(const std::string& path, void** data, unsigned int* width, unsigned int* height);
};
  
}
