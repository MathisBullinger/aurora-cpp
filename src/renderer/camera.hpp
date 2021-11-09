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
  void rotate(const Quaternion& rotation);
  
  const Matrix<4, 4, float>& viewMatrix();
  const Matrix<4, 4, float>& projectionMatrix();

  Vector<3, float> getPosition() const;
  Vector<3, float> getViewDir() const;
  Vector<3, float> getUpDir() const;

private:
  Projection* projection_ = nullptr;
  Matrix<4, 4, float> view_;
  Vector<3, float> position_;
  Quaternion rotation_;
  bool dirty_ = true;
};

}
