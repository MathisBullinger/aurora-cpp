#pragma once

#include <string>
#include <cmath>
#include <utility>
#include <iostream>
#include <tuple>
#include <type_traits>
#include "math/angle.hpp"

namespace aur {

template <const unsigned int dimensions, typename T = float>
class Vector {
public:
  template <typename... U>
  Vector(U... values): values{T(values)...} {}

  template <typename U>
  Vector(const Vector<dimensions, U>& source) {
    for (unsigned int i = 0; i < dimensions; i++) values[i] = (T)source[i];
  }
  
  T values[dimensions] = {0};

  Vector<dimensions, T> operator +(const Vector<dimensions, T>& rhs) const {
    Vector<dimensions, T> result;
    for (unsigned int i = 0; i < dimensions; i++)
      result.values[i] = values[i] + rhs.values[i];
    return result;
  }

  void operator +=(const Vector<dimensions, T>& rhs) {
    for (unsigned int i = 0; i < dimensions; i++)
      values[i] += rhs[i];
  }

  Vector<dimensions, T> operator -(const Vector<dimensions, T>& rhs) const {
    Vector<dimensions, T> result;
    for (unsigned int i = 0; i < dimensions; i++)
      result.values[i] = values[i] - rhs.values[i];
    return result;
  }

  Vector<dimensions, T> operator -() const {
    Vector<dimensions, T> result;
    for (unsigned int i = 0; i < dimensions; i++) result[i] = -values[i];
    return result;
  }

  template <typename N> requires std::is_arithmetic<N>::value
  Vector<dimensions, decltype(std::declval<T&>() * std::declval<N&>())> operator *(N n) const {
    Vector<dimensions, decltype(std::declval<T&>() * std::declval<N&>())> result;
    for (unsigned int i = 0; i < dimensions; i++) 
      result.values[i] = values[i] * n;
    return result;
  }

  template <typename N>
  Vector<dimensions, decltype(std::declval<T&>() * std::declval<N&>())> operator *(const Vector<dimensions, N>& rhs) const {
    Vector<dimensions, decltype(std::declval<T&>() * std::declval<N&>())> result;
    for (unsigned int i = 0; i < dimensions; i++)
      result.values[i] = values[i] * rhs[i];
    return result;
  }

  template <typename N>
  Vector<dimensions, decltype(std::declval<T&>() / std::declval<N&>())> operator /(N n) const {
    Vector<dimensions, decltype(std::declval<T&>() / std::declval<N&>())> result;
    for (unsigned int i = 0; i < dimensions; i++) 
      result.values[i] = values[i] / n;
    return result;
  }

  template <typename N>
  Vector<dimensions, T>& operator *=(N n) {
    for (unsigned int i = 0; i < dimensions; i++) values[i] *= n;
    return *this;
  }

  template <typename N>
  Vector<dimensions, T>& operator *=(const Vector<dimensions, N>& v) {
    for (unsigned int i = 0; i < dimensions; i++) values[i] *= v[i];
    return *this;
  }

  template <typename N>
  Vector<dimensions, T>& operator /=(N n) {
    for (unsigned int i = 0; i < dimensions; i++) values[i] /= n;
    return *this;
  }

  template <unsigned int D> 
  Vector<D, T> fit() {
    Vector<D, T> result;
    for (unsigned int i = 0; i < std::min(D, dimensions); i++) result[i] = values[i];
    return result;
  }

  T operator [](unsigned int i) const {
    return values[i];
  }

  T& operator [](unsigned int i) {
    return values[i];
  }

  T x() const {
    return values[0];
  }

  T y() const requires (dimensions >= 2) {
    return values[1];
  }

  T z() const requires (dimensions >= 3) {
    return values[2];
  }

  T w() const requires (dimensions >= 4) {
    return values[3];
  }

  template <std::size_t I>
  auto& get() {
    return values[I];
  }

  template <std::size_t I>
  const auto& get() const {
    return values[I];
  }

  double magnitude() const {
    double mag = 0;
    for (auto v : values) mag += pow(v, 2);
    return sqrt(mag);
  }

  template <typename N = T>
  Vector<dimensions, N> normal() const {
    return *this / (N)magnitude();
  }

  bool isNormalized() const {
    return abs(magnitude() - 1) < 1e-5;
  }

  template <typename R = T>
  auto dot(const Vector<dimensions, R>& rhs) const {
    decltype(std::declval<T&>() * std::declval<R&>()) product = 0;
    for (unsigned int i = 0; i < dimensions; i++) 
      product += values[i] * rhs.values[i];
    return product;
  }

  template <typename R>
  Vector<3, decltype(std::declval<T&>() * std::declval<R&>())> cross(const Vector<3, R>& rhs) const requires (dimensions == 3) {
    return {
      values[1] * rhs.values[2] - values[2] * rhs.values[1],
      values[2] * rhs.values[0] - values[0] * rhs.values[2],
      values[0] * rhs.values[1] - values[1] * rhs.values[0]
    };
  }

  Vector<2, T> rotate(const angle n) const requires (dimensions == 2) {
    auto s = sin(n);
    auto c = cos(n);
    return {
      values[0] * c - values[1] * s,
      values[0] * s + values[1] * c
    };
  }

  template <const unsigned int D, typename N>
  friend std::ostream& operator<<(std::ostream&, const Vector<D, N>&);
};

template <const unsigned int D, typename T = float>
using vec = Vector<D, T>;

template <typename T = float>
using vec2 = Vector<2, T>;

template <typename T = float>
using vec3 = Vector<3, T>;

template <const unsigned int D, typename N>
std::ostream& operator<<(std::ostream& os, const Vector<D, N>& target) {
  std::cout << "Vec" << D << "<" << typeid(N).name() << ">(";
  for (auto i = 0; i < D; i++) {
    if (i > 0) std::cout << ", ";
    std::cout << target.values[i];
  }
  std::cout << ")";
  return os;
}

template <const unsigned int D, typename T, typename N> requires std::is_arithmetic<N>::value
Vector<D, decltype(std::declval<T&>() * std::declval<N&>())> operator *(N n, const Vector<D, T>& vec) {
  return vec * n;
}

struct Euler : public Vector<3, angle> {
  template <typename T>
  Euler(T yaw, T pitch, T roll) 
    : Vector<3, angle>{angle::radians(yaw), angle::radians(pitch), angle::radians(roll)} {};
  
  angle yaw() /* ψ */ const {
    return values[0];
  }
  
  angle pitch() /* θ */ const {
    return values[1];
  }

  angle roll() /* φ */ const {
    return values[2];
  }
};

}

namespace std {
  template <const unsigned int N, typename T>
  struct tuple_size<aur::Vector<N, T>> : std::integral_constant<std::size_t, N> {};

  template <const unsigned int N, typename T, std::size_t I>
  struct tuple_element<I, aur::Vector<N, T>> {
    using type = T;
  };
}
