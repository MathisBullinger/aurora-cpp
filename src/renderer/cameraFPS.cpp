#include "./camera.hpp"

namespace aur {

void FPSCamera::rotate(const Quaternion& rotation) {
  // todo
}

void FPSCamera::addYaw(angle yaw) {
  yaw_ += yaw;
  dirty_ = true;
}

void FPSCamera::addPitch(angle pitch) {
  pitch_ = std::clamp(pitch_ + pitch, -90_deg, 90_deg);
  dirty_ = true;
}

void FPSCamera::lookIn(const Vector<3, float>& direction) {
  assert(direction.isNormalized());
  auto [x, y, z] = direction;
  auto pSum = abs(x) + abs(y) + abs(z);
  pitch_ = -angle::degrees( 90 * (abs(y) / pSum) );
  if (direction[1] > 0) pitch_ *= -1;
  yaw_ = angle::radians(atan2(-direction[0], -direction[2]));
  dirty_ = true;
}

void FPSCamera::lookAt(const Vector<3, float>& target) {
  lookIn((target - position_).normal());
}

void FPSCamera::roll(angle dr) {
  roll_ += dr;
  dirty_ = true;
}

Vector<3, float> FPSCamera::getDirView() const {
  return { -sin(yaw_) * cos(pitch_), sin(pitch_), -cos(yaw_) * cos(pitch_) };
}

Vector<3, float> FPSCamera::getDirRight() const {
  return { cos(yaw_), 0, -sin(yaw_) };
}

Vector<3, float> FPSCamera::getDirUp() const {
  return getDirRight().cross(getDirView());
}

void FPSCamera::generateView() {
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
