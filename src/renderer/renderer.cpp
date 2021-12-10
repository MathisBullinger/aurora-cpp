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
  fb.bind();
  GLC(glDepthFunc(GL_LEQUAL));
  GLC(glEnable(GL_CULL_FACE));

  auto shader = Shader::get("pbr.vert", "pbr.frag");
  shader->use();
  
  scene.addObject(shader, Mesh::get("donut_complete.obj"), {0,0,0}, {10,10,10}, {});
  scene.addObject(shader, Mesh::get("sphere.obj"), {5,0,-5}, {1,1,1}, {});
  scene.addObject(shader, Mesh::get("wall.obj"), {5,0,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("wall_no_normal.obj"), {7,0,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("cliff.obj"), {5,2,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("cliff_no_normal.obj"), {7,2,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("concrete.obj"), {5,4,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("concrete_no_normal.obj"), {7,4,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("concrete.obj"), {5,-3,5}, {1,1,1}, {{0,1,0}, 125_deg});
  wall = &scene.addObject(shader, Mesh::get("wallBlock.obj"), {-5,0,5}, {1,1,1}, {});
}

Renderer::~Renderer() {
  Texture::deleteTextures();
  Shader::deleteShaders();
  Material::deleteMaterials();
  Mesh::deleteMeshes();
};

void Renderer::render() {
  fb.bind();
  GLC(glClearColor(0, 0, 0, 1));
  GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  GLC(glEnable(GL_DEPTH_TEST));
  
  scene.render();

  fb.unbind();
  GLC(glClearColor(1, 1, 1, 1));
  GLC(glClear(GL_COLOR_BUFFER_BIT));

  screenShader.use();

  screenShader.setUniformArr("kernel", {
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
  });
  screenShader.setUniform("offset", 5.f);
  
  Mesh::get("screen.obj")->bind();
  GLC(glDisable(GL_DEPTH_TEST));
  GLC(glBindTexture(GL_TEXTURE_2D, fb.getAttachment(FB::COLOR)));
  GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

  wall->rotate({{ 0, 1, 0 }, -.05_deg});
}

void Renderer::setWireMode(bool on) {
  GLC(glPolygonMode(GL_FRONT_AND_BACK, on ? GL_LINE : GL_FILL));
}

}
