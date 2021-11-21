#include "renderer.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "util/gl.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>

#include "loaders/texture.hpp"
#include "loaders/mesh.hpp"

namespace aur {

Renderer::Renderer() {
  GLC(glEnable(GL_DEPTH_TEST));
  GLC(glDepthFunc(GL_LESS));
  // GLC(glEnable(GL_CULL_FACE));

  auto shader = Shader::get("basic.vert", "basic.frag");
  shader->use();
  
  boxMesh = new Mesh("../resources/meshes/box.obj");

  float scale = 1;
  scene.addObject(shader, boxMesh, {0,0,0}, {scale,scale,scale}, {});

  loader::Texture textureLoader;
  auto texId = textureLoader.loadBMP("../resources/textures/skybox/front.bmp");
  glBindTexture(GL_TEXTURE_2D, texId);
}

Renderer::~Renderer() {
  Shader::deleteShaders();
  delete boxMesh;
};

void Renderer::render() {
  scene.render();
}

void Renderer::setWireMode(bool on) {
  GLC(glPolygonMode(GL_FRONT_AND_BACK, on ? GL_LINE : GL_FILL));
}

}
