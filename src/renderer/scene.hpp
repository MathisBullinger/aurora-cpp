#pragma once

#include <map>
#include <vector>
#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/camera.hpp"
#include "input/cameraController.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "math/quaternion.hpp"
#include "math/transform.hpp"

namespace aur {

class Object {
public:
  Object(vec3<float> translation, vec3<float> scale, Quaternion rotation);
  const Matrix<4, 4, float>& getModel();

  void rotate(const Quaternion& rotation);

private:
  vec3<float> translation_;
  vec3<float> scale_;
  Quaternion rotation_;

  bool dirty_ = true;
  Matrix<4, 4, float> model_;
};

using RenderGraph = std::map<Shader*, std::map<Mesh*, std::vector<Object>>>;

class Scene {
public:
  Scene();

  void render();

  Object& addObject(
    Shader* shader,
    Mesh* mesh, 
    const vec3<float>& translate, 
    const vec3<float>& scale,
    const Quaternion& rotation
  );

private:
  RenderGraph renderGraph;
  FPSCamera camera;
  std::unique_ptr<CameraController> controller{CameraController::create(camera)};
  Mesh skybox{"cube.obj"};
};
  
}
