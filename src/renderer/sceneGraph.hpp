#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "math/quaternion.hpp"
#include "renderer/mesh.hpp"
#include "renderer/material.hpp"
#include "renderer/shader.hpp"
#include <vector>
#include <optional>

namespace aur::scene {

struct Transform {
  Vector<3, float> translation { 0, 0, 0 };
  Vector<3, float> scale { 1, 1, 1 };
  Quaternion rotation {};
};

class Node {
public:
  Transform transform {};
  Mesh* mesh = nullptr;
  Material* material = nullptr;
  Shader* shader = nullptr;
  bool noShade = false;
  std::vector<Node*> children;
  Node* parent = nullptr;

  void update();
  Matrix<4, 4, float>& worldTransform();
  Vector<3, float> worldPos();

private:
  bool dirty_ = true;
  Matrix<4, 4, float> mm_ {};
};

}
