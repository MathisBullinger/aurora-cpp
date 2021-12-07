#include "renderer.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "util/gl.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include "renderer/texture.hpp"

namespace aur {

Renderer::Renderer() {
  GLC(glEnable(GL_DEPTH_TEST));
  GLC(glDepthFunc(GL_LEQUAL));
  GLC(glEnable(GL_CULL_FACE));

  auto shader = Shader::get("basic.vert", "basic.frag");
  shader->use();
  
  scene.addObject(shader, Mesh::get("donut_complete.obj"), {0,0,0}, {10,10,10}, {});
  scene.addObject(shader, Mesh::get("wall.obj"), {5,0,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("wall.obj"), {5,-3,5}, {1,1,1}, {{0,1,0}, 125_deg});
  wall = &scene.addObject(shader, Mesh::get("wallBlock.obj"), {-5,0,5}, {1,1,1}, {});
}

Renderer::~Renderer() {
  Texture::deleteTextures();
  Shader::deleteShaders();
  Material::deleteMaterials();
  Mesh::deleteMeshes();
};

void Renderer::render() {
  scene.render();

  wall->rotate({{ 0, 1, 0 }, -.05_deg});
}

void Renderer::setWireMode(bool on) {
  GLC(glPolygonMode(GL_FRONT_AND_BACK, on ? GL_LINE : GL_FILL));
}

}
