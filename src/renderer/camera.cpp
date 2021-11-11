#include "./camera.hpp"
#include "math/transform.hpp"

namespace aur {

Camera::Camera() {
  projection_ = new Perspective(8.f/6, 60_deg);
}

Camera::~Camera() {
  delete projection_;
}

void Camera::move(const Vector<3, float>& by) {
  position_ += by;
  dirty_ = true;
}

void Camera::rotate(const Quaternion& rotation) {
  rotation_ = rotation * rotation_;
  dirty_ = true;
}

void Camera::lookIn(const Vector<3, float>& direction) {
  auto a = getViewDir().dot(direction) / direction.magnitude();
  auto axis = abs(a) == 1 ? getUpDir() : getViewDir().cross(direction).normal();
  rotate({ axis, angle::radians(a * PI) });
}

void Camera::lookAt(const Vector<3, float>& target) {
  lookIn(target - getPosition());
}

Vector<3, float> Camera::getPosition() const {
  return position_;
}

Vector<3, float> Camera::getViewDir() const {
  return rotation_ * Vector<3, float>{0, 0, 1};
}

Vector<3, float> Camera::getUpDir() const {
  return rotation_ * Vector<3, float>{0, 1, 0};
}

const Matrix<4, 4, float>& Camera::viewMatrix() {
  if (dirty_) {
    dirty_ = false;
    auto z = getViewDir();
    auto y = -getUpDir();
    auto x = z.cross(y);

    view_.write<3, 4>({
      -x[0], -x[1], -x[2], x.dot(position_),
      -y[0], -y[1], -y[2], y.dot(position_),
      -z[0], -z[1], -z[2], z.dot(position_),
    });
  }
  
  return view_;
}

const Matrix<4, 4, float>& Camera::projectionMatrix() {
  return projection_->getProjection();
}

}
