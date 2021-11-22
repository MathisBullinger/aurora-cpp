#pragma once

#include <string>

namespace aur::loader {

int bmp(const std::string& path, void** data, unsigned int* width, unsigned int* height);
  
}
