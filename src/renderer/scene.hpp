#pragma once

#include <map>
#include <vector>
#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "math/quaternion.hpp"

namespace aur {

class Object {
public:
  Object(vec3<float> translation, vec3<float> scale, Quaternion rotation);
  const Matrix<4, 4, float>& getModel();

private:
  vec3<float> translation;
  vec3<float> scale;
  Quaternion rotation;

  bool dirty = true;
  Matrix<4, 4, float> model;
};

using RenderGraph = std::map<Shader*, std::map<Mesh*, std::vector<Object>>>;

class Scene {
public:
  void render();

  void addObject(
    Shader* shader, 
    Mesh* mesh, 
    const vec3<float>& translate, 
    const vec3<float>& scale,
    const Quaternion& rotation
  );

private:
  RenderGraph renderGraph;
};
  
}
