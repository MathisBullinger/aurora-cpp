#include "./projection.hpp"

namespace aur {

Projection::~Projection() {};

Matrix<4, 4, float>& Projection::getProjection() {
  if (dirty_) {
    compute();
    dirty_ = false;
  }
  
  return projection_;
}

Perspective::Perspective(float aspect, angle fov, float near, float far)
  : aspect_{aspect}, fov_{fov}, near_{near}, far_{far} {};

void Perspective::compute() {
  projection_[{0, 0}] = 1 / (aspect_ * tan(fov_ / 2));
  projection_[{1, 1}] = 1 / tan(fov_ / 2);
  projection_[{2, 2}] = -(far_ + near_) / (far_ - near_);
  projection_[{2, 3}] = -(2 * far_ * near_) / (far_ - near_);
  projection_[{3, 2}] = -1.f;
}
  
}
