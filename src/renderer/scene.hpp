#pragma once

#include <map>
#include <vector>
#include <list>
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
  ~Object();
  const Matrix<4, 4, float>& getModel();
  
  void rotate(const Quaternion& rotation);
  Material* material = nullptr;

private:
  vec3<float> translation_;
  vec3<float> scale_;
  Quaternion rotation_;

  bool dirty_ = true;
  Matrix<4, 4, float> model_;
};

using RenderGraph = std::map<Shader*, std::map<Mesh*, std::vector<Object>>>;

struct Light {
  Vector<3, float> position;
  Vector<3, float> color;
  float strength;
};

class Scene {
public:
  Scene();

  void render();

  Object& addObject(
    Shader* shader,
    Mesh* mesh, 
    const vec3<float>& translate, 
    const vec3<float>& scale = {1,1,1},
    const Quaternion& rotation = {}
  );

  Light* addLight(
    const vec3<float>& pos, 
    const vec3<float>& color, 
    float strength
  );

private:
  RenderGraph renderGraph;
  FPSCamera camera;
  std::unique_ptr<CameraController> controller{CameraController::create(camera)};
  Mesh skybox{"cube.obj"};
  std::list<Light> lights;
};
  
}
