#pragma once

#include <string>

namespace aur::path {

enum base {
  SHADERS,
  TEXTURES,
  MESHES
};

std::string join(std::string a, std::string b);

std::string join(base base, std::string path);

}
