#include "./path.hpp"
#include <filesystem>

namespace aur::path {

std::string basePath_;

std::string basePath() {
  if (basePath_.size()) return basePath_;
  auto path = std::filesystem::current_path();
  while (true) {
    if (std::filesystem::is_directory(std::filesystem::status(std::string(path) + "/resources"))) break;
    auto parent = path.parent_path();
    if (path == parent) throw std::runtime_error("can't find base directory");
    path = parent;
  }
  return basePath_ = std::string(path);
}

std::string join(std::string a, std::string b) {
  if (!a.ends_with('/')) a += '/';
  return a + (b.starts_with('/') ? b.erase(0, 1) : b);
}

std::string resolvePath(base dir) {
  switch (dir) {
    case SHADERS:   return join(basePath(), "resources/shaders/");
    case TEXTURES:  return join(basePath(), "resources/textures/");
    case MESHES:    return join(basePath(), "resources/meshes/");
  }
}

std::string join(base base, std::string path) {
  return join(resolvePath(base), path);
}

}
