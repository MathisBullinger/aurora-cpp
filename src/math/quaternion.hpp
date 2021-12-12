#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "math/angle.hpp"
#include <cmath>
#include <exception>

#define CHECK_NORMAL

namespace aur {

class Quaternion {
public:
  Quaternion(): w{1}, v{0,0,0} {}

  Quaternion(const vec3<float>& axis, angle angle) {
    w = cos(angle / 2);
    v = axis * (float)sin(angle / 2);
  }

  Quaternion inverse() const {
    Quaternion res;
    res.v *= -1;
    return res;
  }

  Quaternion operator *(const Quaternion& rhs) const {
    if (w == rhs.w && v == rhs.v) return *this;
    Quaternion res;
    res.w = w * rhs.w - v.dot(rhs.v);
    res.v = w * rhs.v + v * rhs.w + v.cross(rhs.v);
    return res;
  }

  template <typename T = float>
  vec3<T> operator *(const vec3<T>& vr) const {
    auto v_ = v.cross(vr);
    return vr + v_ * (2*w) + v.cross(v_) * 2;
  }

  Quaternion slerp(const Quaternion& target, float n) {
    const Quaternion& q = *this;
    Quaternion r = target;
    
    float cosOm = w * r.w + r.v.dot(v);

    if (cosOm < 0) {
      r.w = -r.w;
      r.v = -r.v;
      cosOm = -cosOm;
    }

    float k0, k1;
    if (cosOm > 0.9999f) {
      k0 = 1-n;
      k1 = n;
    }
    else {
      float sinOm = sqrt(1 - cosOm * cosOm);

      float flOmega = atan2(sinOm, cosOm);
      float sinOmInv = 1.f / sinOm;

      k0 = sin((1 - n) * flOmega) * sinOmInv;
      k1 = sin(n * flOmega) * sinOmInv;
    }

    Quaternion result;
    result.w = q.w * k0 + r.w * k1;
    result.v = q.v * k0 + r.v * k1;

    return result;
  }

  Matrix<4, 4, float> matrix() const {
    const auto& [x, y, z] = v;
    return {
      w*w + x*x - y*y - z*z,  2*x*y - 2*w*z,          2*x*z + 2*w*y,          0,
      2*x*y + 2*w*z,          w*w - x*x + y*y - z*z,  2*y*z - 2*w*x,          0,
      2*x*z - 2*w*y,          2*y*z + 2*w*x,          w*w - x*x - y*y + z*z,  0,
      0,0,0,1
    };
  }

  Euler euler() const {
    return {
      atan2(2 * (w * v[0] + v[1] * v[2]), 1 - 2 * (v[0] * v[0] + v[1] * v[1])),
      asin(2 * (w * v[1] - v[2] * v[0])),
      atan2(2 * (w * v[2] + v[0] * v[1]), 1 - 2 * (v[1] * v[1] + v[2] * v[2]))
    };
  }

private:
  float w;
  vec3<float> v;
};

}
