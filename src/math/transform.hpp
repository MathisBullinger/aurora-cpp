#pragma once

#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"

namespace aur::transform {

template <unsigned int D, typename T>
Matrix<D+1, D+1, T> translate(const Vector<D, T>& vec) {
  Matrix<D+1, D+1, T> matrix;
  for (unsigned int i = 0; i < D; i++) matrix[{i, D}] = vec[i];
  return matrix;
}

template <unsigned int D, typename T>
Matrix<D+1, D+1, T> scale(const Vector<D, T>& vec) {
  Matrix<D+1, D+1, T> matrix;
  for (unsigned int i = 0; i < D; i++) matrix[{i, i}] = vec[i];
  return matrix;
}

template <unsigned int D, typename T>
Matrix<D+1, D+1, T> scale(T n) {
  Matrix<D+1, D+1, T> matrix;
  for (unsigned int i = 0; i < D; i++) matrix[{i, i}] = n;
  return matrix;
}

template <unsigned int D, typename T>
Matrix<D+1, D+1, T> matrix(
  const Vector<D, T>& translation, 
  const Vector<D, T>& scale,
  const Quaternion& rotation
) {
  return transform::translate(translation) * 
         rotation.matrix() * 
         transform::scale(scale);
}

}
