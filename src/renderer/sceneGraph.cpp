#include "./sceneGraph.hpp"
#include "math/transform.hpp"

namespace aur::scene {

void Node::listen() {
  Node* node = this;
  while (node) {
    node->subbed_ = true;
    node = node->parent;
  }
}

void Node::update() {
  dirty_ = true;
  for (auto child : children) child->update();
}

Matrix<4, 4, float>& Node::worldTransform() {
  if (!dirty_) return mm_;
  mm_ = transform::translate(transform.translation) *
        transform.rotation.matrix() *
        transform::scale(transform.scale);
  if (parent) mm_ = mm_ * parent->worldTransform();
  dirty_ = false;
  std::cout << mm_ << std::endl;
  return mm_;
}

Vector<3, float> Node::worldPos() {
  return worldTransform() * Vector<4, float>{ 0, 0, 0, 1 };
}

}
