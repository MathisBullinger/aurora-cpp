#include "./camera.hpp"

namespace aur {

Camera::Camera() {
  projection_ = new Perspective(8.f/6, 60_deg);
}

Camera::~Camera() {
  delete projection_;
}

void Camera::move(const Vector<3, float>& by) {
  auto pos = getPosition() + by;
  for (unsigned int r = 0; r < 3; r++)
    view_[{r, 3}] = -view_.read<3>(r, 0, matrix::row).dot(pos);
}

void Camera::lookIn(const Vector<3, float>& direction) {
  Vector<3, float> up{0, 1, 0};
  
  auto z = -direction.normal();
  auto x = -z.cross(up).normal();
  auto y = x.cross(-z);
  auto position = getPosition();

  view_.write<3, 4>({
    x[0], x[1], x[2], -x.dot(position),
    y[0], y[1], y[2], -y.dot(position),
    z[0], z[1], z[2], -z.dot(position),
  });
}

void Camera::lookAt(const Vector<3, float>& target) {
  lookIn((target - getPosition()).normal());
}

void Camera::rotate(const Quaternion& rotation) {
  lookIn(rotation * getViewDir());
}

Vector<3, float> Camera::getPosition() const {
  return view_.inverse().read<3>(0, 3, matrix::column);
}

Vector<3, float> Camera::getViewDir() const {
  return -view_.read<3>(2, 0, matrix::row);
}

Vector<3, float> Camera:: getUpDir() const {
  return view_.read<3>(1, 0, matrix::row);
}

const Matrix<4, 4, float>& Camera::viewMatrix() {
  return view_;
}

const Matrix<4, 4, float>& Camera::projectionMatrix() {
  return projection_->getProjection();
}

Matrix<4, 4, float> Camera::calcViewMatrix(
  const Vector<3, float>& position,
  const Vector<3, float>& direction,
  const Vector<3, float>& up
) {
  auto z = -direction.normal();
  auto x = -z.cross(up.normal()).normal();
  auto y = x.cross(-z);

  return {
    x[0], x[1], x[2], -x.dot(position),
    y[0], y[1], y[2], -y.dot(position),
    z[0], z[1], z[2], -z.dot(position),
    0, 0, 0, 1
  };
}

}
