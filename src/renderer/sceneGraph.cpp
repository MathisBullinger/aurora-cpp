#include "./sceneGraph.hpp"
#include "math/transform.hpp"

namespace aur::scene {

void Node::update() {
  if (dirty_) return;
  dirty_ = true;
  for (auto child : children) child->update();
}

Matrix<4, 4, float>& Node::worldTransform() {
  if (!dirty_) return mm_;
  mm_ = transform::matrix(transform.translation, transform.scale, transform.rotation);
  if (parent) mm_ = parent->worldTransform() * mm_;
  dirty_ = false;
  return mm_;
}

Vector<3, float> Node::worldPos() {
  return worldTransform() * Vector<4, float>{ 0, 0, 0, 1 };
}

}
