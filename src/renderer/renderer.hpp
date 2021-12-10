#pragma once

#include "shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/scene.hpp"
#include "renderer/frameBuffer.hpp"

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
  FrameBuffer fb{ 1200, 900, FB::COLOR | FB::DEPTH };
  Shader& screenShader = *Shader::get("screen.vert", "screen.frag");
};

}
