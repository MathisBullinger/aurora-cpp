#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include "util/array.hpp"

#define COLUMN_MAJOR

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

    void set(unsigned int row, unsigned int column, T value) {
      #ifdef COLUMN_MAJOR
        values[column * rows + row] = value;
      #else
        values[row * columns + column] = value;
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

    template <const unsigned int R, const unsigned int C, typename N>
    friend std::ostream& operator<<(std::ostream&, const Matrix<R, C, N>&);
};



template <typename T>
static Matrix<4, 4, T> translation(T x, T y, T z) {
  Matrix<4, 4, T> result;
  result.set(0, 3, x);
  result.set(1, 3, y);
  result.set(2, 3, z);
  return result;
}

static Matrix<4, 4, float> perspective(float aspect, float fov, float near, float far) {
  return Matrix<4, 4, float>({
    1 / (aspect * tan(fov / 2)),  0.f,                0.f,                           0.f,
    0.f,                          1 / tan(fov / 2),   0.f,                           0.f,
    0.f,                          0.f,               -(far + near) / (far - near),  -(2 * far * near) / (far - near),
    0.f,                          0.f,               -1.f,                           0.f,
  });
}

static Matrix<4, 4, float> make_view(
  float rx, float ry, float rz,
  float ux, float uy, float uz,
  float fx, float fy, float fz, 
  float px, float py, float pz
) {
  return Matrix<4, 4, float>({
    rx, ux, fx, px,
    ry, uy, fy, py,
    rz, uz, fz, pz,
    0.f, 0.f, 0.f, 1.f
  });
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
