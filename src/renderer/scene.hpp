#pragma once

#include "./sceneGraph.hpp"
#include "renderer/camera.hpp"
#include "input/cameraController.hpp"
#include "loaders/hjson.hpp"
#include "renderer/cubemap.hpp"

namespace aur {

struct Light {
  scene::Node* node;
  float strength = 10;
  Vector<3, float> color { 1, 1, 1 };
};

class Animation {
public:
  enum class TimeFunc {
    linear,
    sin
  };

  Animation(scene::Node& node, TimeFunc tf);
  virtual ~Animation();
  void execute(float sec);
  virtual void apply(float n) = 0;

protected:
  scene::Node& node_;
  float(*timing_)(float);

private:
  double total_ = 0;
};

class Rotation : public Animation {
public:
  Rotation(scene::Node& node, TimeFunc tf, const Vector<3, float>& axis, float theta);
  void apply(float n);

private:
  Vector<3, float> axis_;
  angle thetaPerSec_;
};

class Roughness : public Animation {
public:
  Roughness(scene::Node& node, TimeFunc tf, float min, float max);
  void apply(float n);

private:
  float min_;
  float max_;
};

class Scene {
public:
  Scene();
  ~Scene();
  scene::Node& addNode(scene::Node* parent = nullptr);
  scene::Node& getGraph();
  Camera& getCamera() const;
  Texture* getSkybox() const;
  std::vector<Animation*>& getAnimations();

  void loadScene(const std::string& path);
  std::vector<Light*>& getLights();

private:
  void loadNode(const hjson::Value& value, scene::Node* parent = nullptr);
  Vector<3, float> vec3(const hjson::Value& value) const;
  Material* createMaterial();

  scene::Node root_;
  std::vector<Light*> lights_;
  std::vector<Animation*> animations_;
  FPSCamera camera_;
  std::unique_ptr<CameraController> cameraController_{CameraController::create(camera_)};
  Texture* skybox_ = nullptr;
  unsigned int mtlC = 0;
};

}
