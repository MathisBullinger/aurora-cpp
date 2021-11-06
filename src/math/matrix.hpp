#pragma once
#include <array>
#include <vector>
#include <string>
#include <cmath>
#include "util/array.hpp"
#include "math/vector.hpp"

#define COLUMN_MAJOR

namespace aur {

namespace matrix {
struct Index {
  unsigned int row;
  unsigned int column;
};
}

template <const unsigned int rows, const unsigned int columns, typename T = double>
class Matrix {
public:
  Matrix() {
    for (unsigned char i = 0; i < std::min(rows, columns); i++)
      values[i * columns + i] = 1;
  }
    
  template <typename... U>
  Matrix(U... rowMajor) : values{T(rowMajor)...} {
    #ifdef COLUMN_MAJOR
      transposeMajor<T>(values, rows, columns);
    #endif
  }

  T values[rows * columns] = {0};

  T operator [](const matrix::Index& i) const {
    #ifdef COLUMN_MAJOR
      return values[i.column * rows + i.row];
    #else
      return values[i.row * columns + i.column];
    #endif
  }

  T& operator [](const matrix::Index& i) {
    #ifdef COLUMN_MAJOR
      return values[i.column * rows + i.row];
    #else
      return values[i.row * columns + i.column];
    #endif
  }

  template <const unsigned int bcols>
  Matrix<rows, bcols, T> operator*(const Matrix<columns, bcols, T>& rhs) {
    Matrix<rows, bcols, T> result({(T)0});

    for (unsigned int i = 0; i < rows * bcols; i++) {
      for (unsigned int e = 0; e < columns; e++) {
        #ifdef COLUMN_MAJOR
          result.values[i] +=
            values[e * rows + i % rows] *
            rhs.values[i / bcols * columns + e];
        #else
          result.values[i] += 
            values[i / bcols * columns + e] * 
            rhs.values[e * bcols + i % bcols];
        #endif
      }
    }

    return result;
  }

  template <typename N>
  Vector<rows, decltype(std::declval<T&>() * std::declval<N&>())> operator *(const Vector<columns, N>& vector) const {
    Vector<rows, decltype(std::declval<T&>() * std::declval<N&>())> result;

    for (unsigned int r = 0; r < rows; r++)
      for (unsigned int c = 0; c < columns; c++)
        result[r] += (*this)[{r,c}] * vector[c];

    return result;
  }

  template <const unsigned int R, const unsigned int C, typename N>
  friend std::ostream& operator<<(std::ostream&, const Matrix<R, C, N>&);
};


namespace matrix {

template <typename T>
static Matrix<4, 4, T> translation(const vec3<T>& vec) {
  auto& [x, y, z] = vec;
  return {
    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1
  };
}

template <typename T>
static Matrix<4, 4, T> scale(const vec3<T>& vec) {
  auto& [x, y, z] = vec;
  return {
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1,
  };
}

}

template <const unsigned int R, const unsigned int C, typename N>
std::ostream& operator<<(std::ostream& os, const Matrix<R, C, N>& target) {
  os << R << "x" << C << " Matrix <" << typeid(N).name() << ">";

  unsigned int maxLength = 0;
  std::vector<std::vector<std::string>> rows;

  for (auto r = 0; r < R; r++) {
    std::vector<std::string> row;
    for (auto c = 0; c < C; c++) {
      #ifdef COLUMN_MAJOR
        auto i = c * R + r;
      #else
        auto i = r * C + c;
      #endif
      std::string str = std::to_string(target.values[i]);
      str.erase(str.find_last_not_of('0') + 1, std::string::npos);
      if (str.ends_with('.')) str.pop_back();
      if (str.starts_with('-')) str.erase(0, 1);
      if (str.size() > maxLength) maxLength = str.size();
      row.push_back(std::to_string(target.values[i]));
    }
    rows.push_back(row);
  }

  for (auto row : rows) {
    os << "\n";
    for (auto v : row) {
      std::string str = v.substr(0, !v.starts_with('-') ? maxLength : maxLength + 1);
      if (str.ends_with('.')) {
        str.pop_back();
        str = " " + str;
      }
      os << (str.starts_with('-') ? " " : "  ") << str;
    }
  }
  
  return os;
}

}
