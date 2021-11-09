#pragma once

#include <type_traits>
#include <variant>
#include "math/constants.hpp"

namespace aur {

struct angle {
  double value;

  template <typename T> requires std::is_arithmetic<T>::value
  constexpr static angle radians(const T n) {
    return angle{(double)n};
  }

  template <typename T> requires std::is_arithmetic<T>::value
  constexpr static angle degree(const T n) {
    return angle{(double)n / 360.f * TAU};
  }

  double degrees() const {
    return value / TAU * 360;
  }

  double radians() const {
    return value;
  }

  operator double() const {
    return value;
  }

  constexpr angle operator + (const angle& rhs) const {
    return angle::radians(value + rhs.value);
  }

  void operator += (const angle& rhs) {
    value += rhs.value;
  }

  constexpr angle operator - (const angle& rhs) const {
    return angle::radians(value - rhs.value);
  }

  constexpr angle operator - () const {
    return angle::radians(-value);
  }

  void operator -= (const angle& rhs) {
    value -= rhs.value;
  }

  template <typename T> requires std::is_arithmetic<T>::value
  constexpr angle operator * (T n) const {
    return angle::radians(value * n);
  }

  template <typename T> requires std::is_arithmetic<T>::value
  void operator *=  (T n) {
    value *= n;
  }

  template <typename T> requires std::is_arithmetic<T>::value
  constexpr angle operator / (T n) const {
    return angle::radians(value / n);
  }

  constexpr double operator / (const angle& rhs) const {
    return value / rhs.value;
  }

  template <typename T> requires std::is_arithmetic<T>::value
  void operator /= (T n) {
    value /= n;
  }
};

constexpr angle operator"" _rad (unsigned long long n) {
  return angle::radians(n);
}
constexpr angle operator"" _rad (long double n) {
  return angle::radians(n);
}

constexpr angle operator"" _deg (unsigned long long n) {
  return angle::degree(n);
}
constexpr angle operator"" _deg (long double n) {
  return angle::degree(n);
}

template <typename T> requires std::is_arithmetic<T>::value
constexpr angle operator * (T n, angle a) {
  return angle::radians(n * a.value);
}

}
