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
#include "math/transform.hpp"
#include "loaders/hjson.hpp"

namespace aur {

Renderer::Renderer() {
  fb.bind();
  GLC(glDepthFunc(GL_LEQUAL));
  GLC(glEnable(GL_CULL_FACE));

  std::uint8_t pxWhite[3]{ 0xff, 0xff, 0xff };
  Texture::add("white", new Texture2D(1, 1, &pxWhite[0]));

  scene.loadScene("scenes/default.scn");
}

Renderer::~Renderer() {
  Texture::deleteTextures();
  Shader::deleteShaders();
  Material::deleteMaterials();
  Mesh::deleteMeshes();
};

void Renderer::render(float dSec) {
  fb.bind();
  GLC(glClearColor(0, 0, 0, 1));
  GLC(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  GLC(glEnable(GL_DEPTH_TEST));
  
  renderNode(scene.getGraph());
  if (scene.getSkybox()) renderSkybox(*scene.getSkybox());

  fb.unbind();
  GLC(glClearColor(1, 1, 1, 1));
  GLC(glClear(GL_COLOR_BUFFER_BIT));

  screenShader.use();

  Mesh::get("screen.obj")->bind();
  GLC(glDisable(GL_DEPTH_TEST));
  GLC(glBindTexture(GL_TEXTURE_2D, fb.getAttachment(FB::COLOR)));
  GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

  for (auto animation : scene.getAnimations()) animation->execute(dSec);
}

void Renderer::renderNode(scene::Node& node, Shader* shader) {
  if (node.noShade) shader = nullptr;
  else if (node.shader && node.shader != shader) {
    shader = node.shader;
    shader->use();
    bindUniforms(*shader);
  }

  if (node.mesh && shader) {
    node.mesh->bind();
    bindUniforms(*shader, node.worldTransform());

    for (auto& [m, ibo] : node.mesh->getMaterials()) {
      auto mtl = node.material ?: m;
      bindUniforms(*shader, *mtl);
      ibo->bind();
      GLC(glDrawElements(GL_TRIANGLES, ibo->count, GL_UNSIGNED_INT, 0));
    }
  }

  for (auto child : node.children) renderNode(*child, shader);
}

void Renderer::renderSkybox(const Texture& texture) {
  Matrix<4, 4> skyView;
  skyView.write<3, 3>(scene.getCamera().viewMatrix());
  Matrix<4, 4> skyVP = scene.getCamera().projectionMatrix() * skyView;

  auto skyboxShader = Shader::get("skybox.vert", "skybox.frag");
  skyboxShader->use();
  skyboxShader->setUniform("transform", skyVP);
  
  texture.bind();
  skyBox.bind();
  auto ind = skyBox.getMaterials().begin()->second;
  ind->bind();
  
  GLC(glCullFace(GL_FRONT));
  GLC(glDrawElements(GL_TRIANGLES, ind->count, GL_UNSIGNED_INT, 0));
  GLC(glCullFace(GL_BACK));
}

void Renderer::bindUniforms(Shader& shader) {
  if (shader == *Shader::get("pbr.vert", "pbr.frag")) {
    shader.setUniform("projection", scene.getCamera().projectionMatrix());
    shader.setUniform("view", scene.getCamera().viewMatrix());
    shader.setUniform("camPos", scene.getCamera().getPosition());
    shader.setUniform("roughness", .5f);
    shader.setUniform("ao", .5f);

    auto& lights = scene.getLights();
    shader.setUniform("lightCount", (unsigned int)lights.size());

    for (int i = 0; i < lights.size(); i++) {
      std::string k = "lights[" + std::to_string(i) + "].";
      assert(lights[i]->node);
      shader.setUniform(k + "position", lights[i]->node->worldPos());
      shader.setUniform(k + "color", lights[i]->color);
      shader.setUniform(k + "strength", lights[i]->strength);
    }
  }
}

void Renderer::bindUniforms(Shader& shader, const Matrix<4, 4, float>& trans) {
  if (shader == *Shader::get("pbr.vert", "pbr.frag")) {
    shader.setUniform("model", trans);
    shader.setUniform("normal", Matrix<3,3>{trans}.inverse().transpose());
  }
  if (shader == *Shader::get("light.vert", "light.frag")) {
    auto MVP = scene.getCamera().projectionMatrix() * scene.getCamera().viewMatrix() * trans;
    shader.setUniform("MVP", MVP);
  }
}

void Renderer::bindUniforms(Shader& shader, const Material& mtl) {
  if (shader == *Shader::get("pbr.vert", "pbr.frag")) {
    shader.setUniform("useAlbedoTexture", mtl.texture != nullptr);
    shader.setUniform("albedo.vertex", mtl.albedo);
    shader.setUniform("useNormalMap", mtl.normalMap != nullptr);
    (mtl.texture ?: Texture::get<Texture2D>("white"))->bind(shader.getTexture("albedo.texture"));
    if (mtl.normalMap) mtl.normalMap->bind(shader.getTexture("normalMap"));
  }
  if (shader == *Shader::get("light.vert", "light.frag")) {
    shader.setUniform("color", mtl.albedo);
  }
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
