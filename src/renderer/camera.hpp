#pragma once

#include "math/angle.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "math/quaternion.hpp"
#include "renderer/projection.hpp"

namespace aur {

class Camera {
public:
  Camera();
  ~Camera();

  void move(const Vector<3, float>& by);
  void lookIn(const Vector<3, float>& direction);
  void lookAt(const Vector<3, float>& target);
  void rotate(const Quaternion& rotation);
  
  const Matrix<4, 4, float>& viewMatrix();
  const Matrix<4, 4, float>& projectionMatrix();

  Vector<3, float> getPosition() const;
  Vector<3, float> getViewDir() const;
  Vector<3, float> getUpDir() const;

private:
  static Matrix<4, 4, float> calcViewMatrix(
    const Vector<3, float>& position,
    const Vector<3, float>& direction,
    const Vector<3, float>& up
  );

  Matrix<4, 4, float> view_ = calcViewMatrix({0,0,0}, {0,0,1}, {0,1,0});
  Projection* projection_ = nullptr;
};

}
