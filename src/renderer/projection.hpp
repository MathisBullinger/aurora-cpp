#pragma once

#include "math/matrix.hpp"
#include "math/angle.hpp"

namespace aur {

class Projection {
public:
  virtual ~Projection();
  Matrix<4, 4, float>& getProjection();

protected:
  Matrix<4, 4, float> projection_{0};
  bool dirty_ = true;

  virtual void compute() = 0;
};

class Perspective : public Projection {
public:
  Perspective(float aspect, angle fov = 90_deg, float near = .2, float far = 800);
  
protected:
  void compute();

private:
  float aspect_;
  float far_;
  float near_;
  angle fov_;
};
  
}
