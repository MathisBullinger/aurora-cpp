#include "./camera.hpp"
#include "math/transform.hpp"

namespace aur {

Camera::Camera() {
  projection_ = new Perspective(8.f/6, 60_deg);
}

Camera::~Camera() {
  delete projection_;
}

const Matrix<4, 4, float>& Camera::viewMatrix() {
  if (dirty_) {
    generateView();
    dirty_ = false;
  }
  return view_;
}

const Matrix<4, 4, float>& Camera::projectionMatrix() {
  return projection_->getProjection();
}

const Vector<3, float> Camera::getPosition() const {
  return position_;
}

void Camera::move(const Vector<3, float>& by) {
  position_ += by;
  dirty_ = true;
}

}
