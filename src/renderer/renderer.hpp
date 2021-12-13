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

  void render(float dSec);
  void setWireMode(bool on);

private:
  void renderNode(scene::Node& node, Shader* shader = nullptr);
  void renderSkybox(const Texture& texture);

  void bindUniforms(Shader& shader);
  void bindUniforms(Shader& shader, const Matrix<4, 4, float>& trans);
  void bindUniforms(Shader& shader, const Material& mtl);

  Scene scene;
  FrameBuffer fb{ 1200, 900, FB::COLOR | FB::DEPTH };
  Shader& screenShader = *Shader::get("screen.vert", "screen.frag");
  Mesh skyBox{"cube.obj"};
};

}
