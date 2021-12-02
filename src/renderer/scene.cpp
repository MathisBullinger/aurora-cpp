#include "./scene.hpp"
#include "util/gl.hpp"
#include "renderer/texture.hpp"
#include "renderer/cubemap.hpp"

namespace aur {

Scene::Scene() {
  camera.move({ 0, 0, 5 });
  camera.lookAt({ 0, 0, 0 });
  
  controller->start();
}

void Scene::render() {
  auto view = camera.viewMatrix();
  auto VP = camera.projectionMatrix() * camera.viewMatrix();

  for (auto& [shader, meshes] : renderGraph) {
    shader->use();

    shader->setUniform("view", camera.viewMatrix());

    shader->setUniform("material.ambient", 1, .5, .31);
    shader->setUniform("material.diffuse", 1, .5, .31);
    shader->setUniform("material.specular", .5, .5, .5);
    shader->setUniform("material.shine", 32);

    auto lp = (camera.viewMatrix() * Vector<4, float>{500, 500, 500, 1});
    shader->setUniform("light.pos", lp.x(), lp.y(), lp.z());
    shader->setUniform("light.ambient", .2, .2, .2);
    shader->setUniform("light.diffuse", .5, .5, .5);
    shader->setUniform("light.specular", 1.f, 1.f, 1.f);

    shader->setUniform("eyePos", camera.getPosition());

    for (auto& [mesh, objects] : meshes) {
      mesh->bind();

      for (auto& obj : objects) {
        auto model = obj.getModel();
        auto MVP = VP * model;
        auto normal = Matrix<3, 3>{view * model}.inverse().transpose();

        shader->setUniform("model", model);
        shader->setUniform("MVP", MVP);
        shader->setUniform("normal", normal);

        GLC(glDrawElements(GL_TRIANGLES, mesh->countIndices(), GL_UNSIGNED_INT, 0));
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
  
  GLC(glCullFace(GL_FRONT));
  GLC(glDrawElements(GL_TRIANGLES, skybox.countIndices(), GL_UNSIGNED_INT, 0));
  GLC(glCullFace(GL_BACK));
}

void Scene::addObject(
  Shader* shader, 
  Mesh* mesh, 
  const vec3<float>& translate, 
  const vec3<float>& scale,
  const Quaternion& rotation
) {
  renderGraph[shader][mesh].push_back({translate, scale, rotation});
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
