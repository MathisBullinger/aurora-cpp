#pragma once

#include "shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/object.hpp"
#include <vector>
#include <map>

namespace aur {

class Renderer {
public:
  Renderer();
  ~Renderer();
  void render();

  void setWireMode(bool on);

private:
  bool glLogCall(const char* fun, const char* file, unsigned int line);

  int mvpID;
  int modelID;

  std::map<Mesh*, std::vector<Object*>> objects;
  Shader* shader = nullptr;
};

}
