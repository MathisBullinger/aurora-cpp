#pragma once

#include "shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/scene.hpp"
#include "renderer/frameBuffer.hpp"
#include <chrono>

namespace aur {

class Renderer {
public:
  Renderer();
  ~Renderer();

  void render();
  void setWireMode(bool on);

private:
  Vector<3, float> lightPos(unsigned int n, unsigned int i, float r, float s);

  Scene scene;
  Object* wall;
  FrameBuffer fb{ 1200, 900, FB::COLOR | FB::DEPTH };
  Shader& screenShader = *Shader::get("screen.vert", "screen.frag");
  
  Vector<3, float> lp{ 15, -10, -10 };
  std::vector<Light*> lights;
  const std::chrono::steady_clock::time_point t0 = std::chrono::high_resolution_clock::now();
};

}
