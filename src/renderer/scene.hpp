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
  Animation(scene::Node& node);
  virtual ~Animation();
  virtual void execute(float sec) = 0;

protected:
  scene::Node& node_;
};

class Rotation : public Animation {
public:
  Rotation(scene::Node& node, const Vector<3, float>& axis, float theta);
  void execute(float sec);

private:
  Vector<3, float> axis_;
  angle thetaPerSec_;
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

  scene::Node root_;
  std::vector<Light*> lights_;
  std::vector<Animation*> animations_;
  FPSCamera camera_;
  std::unique_ptr<CameraController> cameraController_{CameraController::create(camera_)};
  Texture* skybox_ = nullptr;
};

}
