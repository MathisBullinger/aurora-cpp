#include "./scene.hpp"
#include "util/gl.hpp"
#include "renderer/texture.hpp"
#include "renderer/cubemap.hpp"

namespace aur {

Scene::Scene() {
  camera.move({ 0, 0, -5 });
  camera.lookAt({ 0, 0, 0 });

  lights.push_back({ { -200, 200, -200 }, { 1, 1, 1 }, 1000000 });
  lights.push_back({ { 200, 200, -200 }, { 1, 0, 0 }, 5 });
  lights.push_back({ { -200, -200, -200 }, { 1, 0, 0 }, 5 });
  lights.push_back({ { 200, -200, -200 }, { 1, 0, 0 }, 5 });
  
  controller->start();
}

void Scene::render() {
  auto view = camera.viewMatrix();

  for (auto& [shader, meshes] : renderGraph) {
    shader->use();

    shader->setUniform("projection", camera.projectionMatrix());
    shader->setUniform("view", camera.viewMatrix());
    shader->setUniform("camPos", camera.getPosition());

    for (unsigned int i = 0; i < lights.size(); i++) {
      std::string k = "lights[" + std::to_string(i) + "].";
      shader->setUniform(k + "position", lights[i].position);
      shader->setUniform(k + "color", lights[i].color);
      shader->setUniform(k + "strength", lights[i].strength);
    }
    shader->setUniform("lightCount", (unsigned int)lights.size());

    shader->setUniform("metallic", .5f);
    shader->setUniform("roughness", .5f);
    shader->setUniform("ao", .5f);

    for (auto& [mesh, objects] : meshes) {
      mesh->bind();

      for (auto& obj : objects) {
        auto model = obj.getModel();

        shader->setUniform("model", model);
        shader->setUniform("normal", Matrix<3,3>{model}.inverse().transpose());

        for (auto& [mtl, indexBuffer] : mesh->getMaterials()) {
          shader->setUniform("useAlbedoTexture", mtl->texture != nullptr);
          (mtl->texture ?: Texture::get<Texture2D>("white"))->bind(shader->getTexture("albedo.texture"));
          shader->setUniform("albedo.vertex", mtl->albedo);

          shader->setUniform("useNormalMap", mtl->normalMap != nullptr);
          if (mtl->normalMap) mtl->normalMap->bind(shader->getTexture("normalMap"));

          indexBuffer->bind();
          GLC(glDrawElements(GL_TRIANGLES, indexBuffer->count, GL_UNSIGNED_INT, 0));
        }
      }
    }
  }


  Matrix<4, 4> skyView;
  skyView.write<3, 3>(camera.viewMatrix());
  Matrix<4, 4> skyVP = camera.projectionMatrix() * skyView;

  auto skyboxShader = Shader::get("skybox.vert", "skybox.frag");
  skyboxShader->use();
  skyboxShader->setUniform("transform", skyVP);
  
  Texture::get<Cubemap>("skybox/")->bind();
  skybox.bind();
  auto ind = skybox.getMaterials().begin()->second;
  ind->bind();
  
  GLC(glCullFace(GL_FRONT));
  GLC(glDrawElements(GL_TRIANGLES, ind->count, GL_UNSIGNED_INT, 0));
  GLC(glCullFace(GL_BACK));
}

Object& Scene::addObject(
  Shader* shader, 
  Mesh* mesh, 
  const vec3<float>& translate, 
  const vec3<float>& scale,
  const Quaternion& rotation
) {
  renderGraph[shader][mesh].push_back({translate, scale, rotation});
  return renderGraph[shader][mesh].back();
}


Object::Object(vec3<float> translation, vec3<float> scale, Quaternion rotation)
  : translation_{translation}, scale_{scale}, rotation_{rotation} {}

const Matrix<4, 4, float>& Object::getModel() {
  if (dirty_) {
    model_ = transform::translate(translation_) * rotation_.matrix() * transform::scale(scale_);
    dirty_ = false;
  }
  return model_;
}

void Object::rotate(const Quaternion& rotation) {
  rotation_ = rotation * rotation_;
  dirty_ = true;
}

}
