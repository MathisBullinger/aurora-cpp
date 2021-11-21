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

  const Matrix<4, 4, float>& projectionMatrix();
  const Matrix<4, 4, float>& viewMatrix();

  void move(const Vector<3, float>& by);
  virtual void lookIn(const Vector<3, float>& direction) = 0;
  virtual void lookAt(const Vector<3, float>& direction) = 0;
  virtual void rotate(const Quaternion& rotation) = 0;

  const Vector<3, float> getPosition() const;
  virtual Vector<3, float> getDirView() const = 0;
  virtual Vector<3, float> getDirUp() const = 0;
  virtual Vector<3, float> getDirRight() const = 0;

protected:
  virtual void generateView() = 0;

  Projection* projection_ = nullptr;
  Matrix<4, 4, float> view_;
  Vector<3, float> position_;
  bool dirty_ = true;
  const Vector<3, float> initialZ{ 0, 0, -1 };
  const Vector<3, float> initialY{ 0, 1, 0 };
};

class FreeCamera : public Camera {
public:
  void rotate(const Quaternion& rotation);
  void lookIn(const Vector<3, float>& direction);
  void lookAt(const Vector<3, float>& target);
  
  Vector<3, float> getDirView() const;
  Vector<3, float> getDirUp() const;
  Vector<3, float> getDirRight() const;

private:
  void generateView();
  Quaternion rotation_;
};

class FPSCamera : public Camera {
public:
  void rotate(const Quaternion& rotation);
  void addYaw(angle yaw);
  void addPitch(angle pitch);
  void lookIn(const Vector<3, float>& direction);
  void lookAt(const Vector<3, float>& target);
  void roll(angle dr);
  
  Vector<3, float> getDirView() const;
  Vector<3, float> getDirUp() const;
  Vector<3, float> getDirRight() const;

private:
  void generateView();
  angle pitch_ = 0_rad;
  angle yaw_ = 0_rad;
  angle roll_ = 0_rad;
};

}
