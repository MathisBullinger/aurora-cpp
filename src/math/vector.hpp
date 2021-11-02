#pragma once

#include <string>
#include <cmath>
#include <utility>
#include <iostream>

namespace aur {

template <const unsigned int dimensions, typename T = double>
class Vector {
  public:
    template <typename... U>
    Vector(U... values): values{T(values)...} {}
  
    T values[dimensions] = {0};

    Vector<dimensions, T> operator +(const Vector<dimensions, T>& rhs) const {
      Vector<dimensions, T> result;
      for (unsigned int i = 0; i < dimensions; i++)
        result.values[i] = values[i] + rhs.values[i];
      return result;
    }

    Vector<dimensions, T> operator -(const Vector<dimensions, T>& rhs) const {
      Vector<dimensions, T> result;
      for (unsigned int i = 0; i < dimensions; i++)
        result.values[i] = values[i] - rhs.values[i];
      return result;
    }

    template <typename N>
    Vector<dimensions, decltype(std::declval<T&>() * std::declval<N&>())> operator *(N n) const {
      Vector<dimensions, decltype(std::declval<T&>() * std::declval<N&>())> result;
      for (unsigned int i = 0; i < dimensions; i++) 
        result.values[i] = values[i] * n;
      return result;
    }

    template <typename N>
    Vector<dimensions, decltype(std::declval<T&>() / std::declval<N&>())> operator /(N n) const {
      Vector<dimensions, decltype(std::declval<T&>() / std::declval<N&>())> result;
      for (unsigned int i = 0; i < dimensions; i++) 
        result.values[i] = values[i] / n;
      return result;
    }

    Vector<dimensions, T>& operator*=(T n) {
      for (unsigned int i = 0; i < dimensions; i++) values[i] *= n;
      return *this;
    }

    T operator [](unsigned int i) const {
      return values[i];
    }

    T& operator [](unsigned int i) {
      return values[i];
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

    template <typename R>
    double dot(const Vector<dimensions, R>& rhs) const {
      double product = 0;
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

    template <const unsigned int D, typename N>
    friend std::ostream& operator<<(std::ostream&, const Vector<D, N>&);
};

template <const unsigned int D, typename T = double>
using vec = Vector<D, T>;

template <typename T = double>
using vec2 = Vector<2, T>;

template <typename T = double>
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

}
