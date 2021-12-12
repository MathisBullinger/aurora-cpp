#pragma once

#include "./sceneGraph.hpp"
#include "renderer/camera.hpp"
#include "input/cameraController.hpp"
#include "loaders/hjson.hpp"

namespace aur {

struct Light {
  scene::Node* node;
  float strength = 10;
  Vector<3, float> color { 1, 1, 1 };
};

class Scene {
public:
  Scene();
  ~Scene();
  scene::Node& addNode(scene::Node* parent = nullptr);
  const scene::Node& getGraph() const;
  Camera& getCamera() const;

  void loadScene(const std::string& path);
  std::vector<Light*>& getLights();

private:
  void loadNode(const hjson::Value& value, scene::Node* parent = nullptr);
  Vector<3, float> vec3(const hjson::Value& value) const;

  scene::Node root_;
  std::vector<Light*> lights_;
  FPSCamera camera_;
  std::unique_ptr<CameraController> cameraController_{CameraController::create(camera_)};
};

}
