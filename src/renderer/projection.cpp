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
  projection_.write<4, 4>({
    1 / (aspect_ * tanf(fov_ / 2)),  0,                   0,                                 0,
    0,                               1 / tanf(fov_ / 2),  0,                                 0,
    0,                               0,                   -(far_ + near_) / (far_ - near_),  -(2 * far_ * near_) / (far_ - near_),
    0,                               0,                   -1,                                0
  });
}

}
