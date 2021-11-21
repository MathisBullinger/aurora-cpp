#include "./camera.hpp"

namespace aur {

void FreeCamera::lookIn(const Vector<3, float>& direction) {
  auto a = getDirView().dot(direction) / direction.magnitude();
  auto axis = abs(a) == 1 ? getDirUp() : getDirView().cross(direction).normal();
  rotate({ axis, angle::radians(a * PI) });
  dirty_ = true;
}

void FreeCamera::lookAt(const Vector<3, float>& target) {
  lookIn(target - getPosition());
}

void FreeCamera::rotate(const Quaternion& rotation) {
  rotation_ = rotation * rotation_;
  dirty_ = true;
}

Vector<3, float> FreeCamera::getDirView() const {
  return rotation_ * initialZ;
}

Vector<3, float> FreeCamera::getDirUp() const {
  return rotation_ * initialY;
}

Vector<3, float> FreeCamera::getDirRight() const {
  return rotation_ * initialZ.cross(initialY);
}


void FreeCamera::generateView() {
  auto x = getDirRight();
  auto y = getDirUp();
  auto z = -getDirView();

  view_.write<3, 4>({
    x[0], x[1], x[2], -x.dot(position_),
    y[0], y[1], y[2], -y.dot(position_),
    z[0], z[1], z[2], -z.dot(position_),
  });
}

}
