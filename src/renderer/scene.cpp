#include "./scene.hpp"
#include "util/path.hpp"

namespace aur {

Scene::Scene() {
  camera_.move({ 0, 0, -5 });
  camera_.lookAt({ 0, 0, 0 });
  root_.shader = Shader::get("pbr.vert", "pbr.frag");
  cameraController_->start();
}

Scene::~Scene() {
  for (auto light : lights_) delete light;
}

scene::Node& Scene::addNode(scene::Node* parent) {
  if (!parent) parent = &root_;
  scene::Node* node = new scene::Node();
  parent->children.push_back(node);
  node->parent = parent;
  return *node;
}

const scene::Node& Scene::getGraph() const {
  return root_;
}

Camera& Scene::getCamera() const {
  return *(Camera*)&camera_;
}

void Scene::loadScene(const std::string& path) {
  auto data = hjson::load(path::join(path::RESOURCES, path));
  loadNode(*data);
}

void Scene::loadNode(const hjson::Value& value, scene::Node* parent) {
  auto data = value.get<hjson::object>();
  
  auto& node = addNode(parent);

  if (data.contains("translate"))
    node.transform.translation = vec3(*data["translate"]);

  if (data.contains("scale"))
    node.transform.scale = data["scale"]->type == hjson::number
      ? Vector<3, float>::filled(data["scale"]->get<hjson::number>())
      : vec3(*data["scale"]);

  if (data.contains("rotate")) {
    auto values = data["rotate"]->get<hjson::array>();
    node.transform.rotation = {
      vec3(*values[0]).normal(),
      angle::degrees(values[1]->get<hjson::number>())
    };
  }
  
  if (data.contains("mesh"))
    node.mesh = Mesh::get(data["mesh"]->get<hjson::string>());

  if (data.contains("shader")) {
    if (data["shader"]->type == hjson::string) {}

    if (data["shader"]->type == hjson::array) {
      auto values = data["shader"]->get<hjson::array>();
      node.shader = Shader::get(
        values[0]->get<hjson::string>(),
        values[1]->get<hjson::string>()
      );
    }
  }

  if (data.contains("light")) {
    Light* light = new Light();
    light->node = &node;

    auto params = data["light"]->get<hjson::object>();

    if (params.contains("strength")) 
      light->strength = params["strength"]->get<hjson::number>();

    lights_.push_back(light);
    node.listen();
  }

  if (data.contains("children")) {
    for (auto child : data["children"]->get<hjson::array>()) {
      loadNode(*child, &node);
    }
  }
}

Vector<3, float> Scene::vec3(const hjson::Value& value) const {
  assert(value.type == hjson::array);
  Vector<3, float> result;
  auto values = value.get<hjson::array>();
  assert(values.size() == 3);
  for (unsigned int i = 0; i < 3; i++) {
    assert(values[i]->type == hjson::number);
    result[i] = values[i]->get<hjson::number>();
  }
  return result;
}

std::vector<Light*>& Scene::getLights() {
  return lights_;
}

}
