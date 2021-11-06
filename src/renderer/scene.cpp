#include "./scene.hpp"
#include "./viewUtil.hpp"
#include "util/gl.hpp"

namespace aur {

void Scene::render() {
  auto view = aur::lookAt({0, 4, -8}, {0, 1.5, 0}, {0, 1, 0});
  auto projection = aur::perspective(800.f/600, M_PI / 4, 0.1f, 1000.f);
  auto VP = projection * view;
  
  for (auto& [shader, meshes] : renderGraph) {
    shader->use();

    for (auto& [mesh, objects] : meshes) {
      // ...bind mesh

      for (auto& obj : objects) {
        auto model = obj.getModel();
        auto MVP = VP * model;

        shader->setUniformMatrix("MVP", MVP);
        shader->setUniformMatrix("model", model);

        GLC(glDrawElements(GL_TRIANGLES, mesh->countIndices(), GL_UNSIGNED_INT, 0));
      }
    }
  }
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
  : translation{translation}, scale{scale}, rotation{rotation} {}

const Matrix<4, 4, float>& Object::getModel() {
  if (dirty) {
    model = matrix::translation(translation) * rotation.matrix() * matrix::scale(scale);
    dirty = false;
  }
  return model;
}

}
