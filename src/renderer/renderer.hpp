#pragma once

#include "shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/scene.hpp"

namespace aur {

class Renderer {
public:
  Renderer();
  ~Renderer();

  void render();
  void setWireMode(bool on);

private:
  Scene scene;
  Object* wall;
};

}
