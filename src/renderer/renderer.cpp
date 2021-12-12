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

  // auto& a = scene.addNode();
  // a.mesh = Mesh::get("donut_complete.obj");
  // a.transform.scale = { 10, 10, 10 };

  scene.loadScene("scenes/default.scn");

  // scene.addObject(shader, Mesh::get("donut_complete.obj"), {0,0,0}, {10,10,10});
  // scene.addObject(shader, Mesh::get("sphere.obj"), {5,0,-5});
  // scene.addObject(shader, Mesh::get("wall.obj"), {5,0,5}, {1,1,1}, {{0,1,0}, 180_deg});
  // scene.addObject(shader, Mesh::get("wall_no_normal.obj"), {7,0,5}, {1,1,1}, {{0,1,0}, 180_deg});
  // scene.addObject(shader, Mesh::get("cliff.obj"), {5,2,5}, {1,1,1}, {{0,1,0}, 180_deg});
  // scene.addObject(shader, Mesh::get("cliff_no_normal.obj"), {7,2,5}, {1,1,1}, {{0,1,0}, 180_deg});
  // scene.addObject(shader, Mesh::get("concrete.obj"), {5,4,5}, {1,1,1}, {{0,1,0}, 180_deg});
  // scene.addObject(shader, Mesh::get("concrete_no_normal.obj"), {7,4,5}, {1,1,1}, {{0,1,0}, 180_deg});
  // scene.addObject(shader, Mesh::get("concrete.obj"), {5,-3,5}, {1,1,1}, {{0,1,0}, 125_deg});
  // wall = &scene.addObject(shader, Mesh::get("wallBlock.obj"), {-5,0,5});

  // scene.addLight({ -200, 200, -450 }, { 1, 1, 1 }, 2e6);
  
  // {
  //   auto mtl = new Material();
  //   mtl->metallic = 0;
  //   scene.addObject(shader, Mesh::get("sphere.obj"), lp, {.5, 1, .5}, {}).material = mtl;
  // }
  // {
  //   auto mtlA = new Material();
  //   mtlA->metallic = 0;
  //   scene.addObject(shader, Mesh::get("wall.obj"), lp + Vector<3,float>{1.5,0,-5}).material = mtlA;
  // }

  // float colors[6][3]{{1,0,0}, {0,1,0}, {0,0,1}, {1,0,0}, {0,1,0}, {0,0,1}};
  // for (int i = 0; i < 6; i++)
  //   lights.push_back(scene.addLight(lp + lightPos(6, i, 1.5, 0), { colors[i][0], colors[i][1], colors[i][2] }, 10));
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
  
  renderNode(scene.getGraph());

  fb.unbind();
  GLC(glClearColor(1, 1, 1, 1));
  GLC(glClear(GL_COLOR_BUFFER_BIT));

  screenShader.use();

  Mesh::get("screen.obj")->bind();
  GLC(glDisable(GL_DEPTH_TEST));
  GLC(glBindTexture(GL_TEXTURE_2D, fb.getAttachment(FB::COLOR)));
  GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

  // float td = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t0).count() / 1000.f;
  // wall->rotate({{ 0, 1, 0 }, -.05_deg});
  // for (int i = 0; i < lights.size(); i++)
  //   lights[i]->position = lp + lightPos(lights.size(), i, 1.5, td);
}

void Renderer::renderNode(
  const scene::Node& node,
  scene::Transform transform,
  Shader* shader
) {
  if (node.shader && node.shader != shader) {
    shader = node.shader;
    shader->use();
    bindUniforms(*shader);
  }

  transform.translation += node.transform.translation;
  transform.scale *= node.transform.scale;
  transform.rotation = node.transform.rotation * transform.rotation;

  // render
  if (node.mesh) {
    node.mesh->bind();
    bindUniforms(*shader, transform);

    for (auto& [mtl, ibo] : node.mesh->getMaterials()) {
      bindUniforms(*shader, *mtl);
      ibo->bind();
      GLC(glDrawElements(GL_TRIANGLES, ibo->count, GL_UNSIGNED_INT, 0));
    }
  }

  for (auto child : node.children) renderNode(*child, transform, shader);
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

void Renderer::bindUniforms(Shader& shader, const scene::Transform& trans) {
  if (shader == *Shader::get("pbr.vert", "pbr.frag")) {
   auto model = 
      transform::translate(trans.translation) * 
      trans.rotation.matrix() * 
      transform::scale(trans.scale);
    
    shader.setUniform("model", model);
    shader.setUniform("normal", Matrix<3,3>{model}.inverse().transpose());
  }
}

void Renderer::bindUniforms(Shader& shader, const Material& mtl) {
  if (shader == *Shader::get("pbr.vert", "pbr.frag")) {
    shader.setUniform("useAlbedoTexture", mtl.texture != nullptr);
    shader.setUniform("albedo.vertex", mtl.albedo);
    // shader.setUniform("useNormalMap", mtl.normalMap != nullptr);
    (mtl.texture ?: Texture::get<Texture2D>("white"))->bind(shader.getTexture("albedo.texture"));
    // if (mtl.normalMap) mtl.normalMap->bind(shader.getTexture("normalMap"));
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
