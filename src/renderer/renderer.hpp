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
  void renderNode(
    const scene::Node& node,
    scene::Transform transform = {},
    Shader* shader = nullptr
  );

  void bindUniforms(Shader& shader);
  void bindUniforms(Shader& shader, const scene::Transform& trans);
  void bindUniforms(Shader& shader, const Material& mtl);

  Vector<3, float> lightPos(unsigned int n, unsigned int i, float r, float s);

  Scene scene;
  FrameBuffer fb{ 1200, 900, FB::COLOR | FB::DEPTH };
  Shader& screenShader = *Shader::get("screen.vert", "screen.frag");
  
  const std::chrono::steady_clock::time_point t0 = std::chrono::high_resolution_clock::now();
};

}
