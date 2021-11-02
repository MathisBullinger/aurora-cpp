#pragma once

#include "math/matrix.hpp"
#include "math/vector.hpp"

#include <iostream>

namespace aur {

using VT = float;
using GLMatrix = Matrix<4, 4, VT>;

static auto perspective(VT aspect, VT fov, VT near, VT far) {
  return GLMatrix({
    1 / (aspect * tan(fov / 2)),  0.f,                0.f,                           0.f,
    0.f,                          1 / tan(fov / 2),   0.f,                           0.f,
    0.f,                          0.f,               -(far + near) / (far - near),  -(2 * far * near) / (far - near),
    0.f,                          0.f,               -1.f,                           0.f,
  });
}

static GLMatrix lookAt(
  const Vector<3, VT>& eye,
  const Vector<3, VT>& at,
  const Vector<3, VT>& up) 
{
  auto zAxis = (at - eye).normal();
  auto xAxis = zAxis.cross(up).normal();
  auto yAxis = xAxis.cross(zAxis);
  zAxis *= -1;
  
  return GLMatrix({
    xAxis.values[0], xAxis.values[1], xAxis.values[2], -xAxis.dot(eye),
    yAxis.values[0], yAxis.values[1], yAxis.values[2], -yAxis.dot(eye),
    zAxis.values[0], zAxis.values[1], zAxis.values[2], -zAxis.dot(eye),
    0, 0, 0, 1
  });
}

}
