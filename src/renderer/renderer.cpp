#include "renderer.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "util/gl.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <GL/glew.h>
#include "renderer/texture.hpp"

namespace aur {

Renderer::Renderer() {
  fb.bind();
  GLC(glDepthFunc(GL_LEQUAL));
  GLC(glEnable(GL_CULL_FACE));

  std::uint8_t pxWhite[3]{ 0xff, 0xff, 0xff };
  Texture::add("white", new Texture2D(1, 1, &pxWhite[0]));

  auto shader = Shader::get("pbr.vert", "pbr.frag");
  shader->use();
  
  scene.addObject(shader, Mesh::get("donut_complete.obj"), {0,0,0}, {10,10,10});
  scene.addObject(shader, Mesh::get("sphere.obj"), {5,0,-5});
  scene.addObject(shader, Mesh::get("wall.obj"), {5,0,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("wall_no_normal.obj"), {7,0,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("cliff.obj"), {5,2,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("cliff_no_normal.obj"), {7,2,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("concrete.obj"), {5,4,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("concrete_no_normal.obj"), {7,4,5}, {1,1,1}, {{0,1,0}, 180_deg});
  scene.addObject(shader, Mesh::get("concrete.obj"), {5,-3,5}, {1,1,1}, {{0,1,0}, 125_deg});
  wall = &scene.addObject(shader, Mesh::get("wallBlock.obj"), {-5,0,5});

  scene.addLight({ -200, 200, -450 }, { 1, 1, 1 }, 2e5);
  
  scene.addObject(shader, Mesh::get("sphere.obj"), lp, {.5, 1, .5}, {});
  scene.addObject(shader, Mesh::get("wall.obj"), lp + Vector<3,float>{1.5,0,-5});

  float colors[6][3]{{1,0,0}, {0,1,0}, {0,0,1}, {1,0,0}, {0,1,0}, {0,0,1}};
  for (int i = 0; i < 6; i++)
    lights.push_back(scene.addLight(lp + lightPos(6, i, 1.5, 0), { colors[i][0], colors[i][1], colors[i][2] }, 10));
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

  Mesh::get("screen.obj")->bind();
  GLC(glDisable(GL_DEPTH_TEST));
  GLC(glBindTexture(GL_TEXTURE_2D, fb.getAttachment(FB::COLOR)));
  GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

  float td = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count() / 1000.f;
  wall->rotate({{ 0, 1, 0 }, -.05_deg});
  for (int i = 0; i < lights.size(); i++)
    lights[i]->position = lp + lightPos(lights.size(), i, 1.5, td);
}

void Renderer::setWireMode(bool on) {
  GLC(glPolygonMode(GL_FRONT_AND_BACK, on ? GL_LINE : GL_FILL));
}

Vector<3, float> Renderer::lightPos(unsigned int n, unsigned int i, float r, float s) {
  return Vector<3, float>{
    sinf(2 * PI / n * i + 10_deg * s) * r,
    0,
    cosf(2 * PI / n * i + 10_deg * s) * r,
  };
}

}
