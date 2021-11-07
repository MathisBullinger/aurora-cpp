#pragma once
#include <array>
#include <vector>
#include <string>
#include <cmath>
#include <type_traits>
#include <concepts>
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

template <unsigned int rows, unsigned int columns, typename T = float>
class Matrix {
public:
  Matrix() {
    for (unsigned char i = 0; i < std::min(rows, columns); i++)
      values[i * columns + i] = 1;
  }

  template <typename... U>
  Matrix(U... rowMajor) : values{(T)rowMajor...} {
    #ifdef COLUMN_MAJOR
      transposeMajor<T>(values, rows, columns);
    #endif
  }

  template <unsigned int R, unsigned C, typename S>
  Matrix(const Matrix<R, C, S>& source) {
    for (unsigned int r = 0; r < std::min(rows, R); r++) 
      for (unsigned int c = 0; c < std::min(columns, C); c++)
        (*this)[{r,c}] = source[{r, c}];
  }

  T values[rows * columns] = {};

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

  T at(const unsigned int row, const unsigned int column) const {
    #ifdef COLUMN_MAJOR
      return values[column * rows + row];
    #else
      return values[row * columns + column];
    #endif
  }

  template <unsigned int bcols>
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
        result[r] += at(r, c) * vector[c];

    return result;
  }

  template <typename N> requires std::is_arithmetic<N>::value
  Matrix<rows, columns, decltype(std::declval<T&>() * std::declval<N&>())> operator *(N n) const {
    Matrix<rows, columns, decltype(std::declval<T&>() * std::declval<N&>())> result;

    for (unsigned int r = 0; r < rows; r++)
      for (unsigned int c = 0; c < columns; c++)
        result[{r,c}] = at(r, c) * n;

    return result;
  }


  template <unsigned int R, unsigned int C>
  Matrix<R, C, T> resize() const {
    return *this;
  }

  T determinant() const requires (rows == columns) {
    if constexpr (rows <= 2) return at(0,0) * at(1,1) - at(0,1) * at(1,0);
    else {
      T n = 0;
      for (unsigned int i = 0; i < columns; i++)
        n += at(0, i) * remove(0, i).determinant() * (i % 2 ? -1 : 1);
      return n;
    }
  }

  template <typename N = T>
  auto inverse() const requires (rows == columns) {
    auto d = determinant();
    assert(d != 0);
    Matrix<columns, rows, typename std::conditional<std::is_floating_point<N>::value, N, float>::type> adjugate{0};

    for (unsigned int r = 0; r < rows; r++)
      for (unsigned int c = 0; c < columns; c++)
        adjugate[{c,r}] = remove(r, c).determinant() * ((r % 2 + c % 2) % 2 ? -1 : 1);

    return 1 / d * adjugate;
  }

  Matrix<rows - 1, columns - 1, T> remove(unsigned int row, unsigned int column) const {
    Matrix<rows - 1, columns - 1, T> sub{0};

    for (unsigned int r = 0; r < rows - 1; r++)
      for (unsigned int c = 0; c < columns - 1; c++)
        sub[{r,c}] = at(r < row ? r : r + 1, c < column ? c : c + 1);

    return sub;
  }

  Matrix<columns, rows, T> transpose() const {
    Matrix<columns, rows, T> result{0};
    for (unsigned int r = 0; r < rows; r++)
      for (unsigned int c = 0; c < rows; c++)
        result[{c,r}] = at(r, c);
    return result;
  }

  template <unsigned int R, unsigned int C, typename N>
  friend std::ostream& operator<<(std::ostream&, const Matrix<R, C, N>&);
};

template <unsigned int R, unsigned int C, typename T, typename N> requires std::is_arithmetic<N>::value
auto operator *(N n, const Matrix<R, C, T>& matrix) {
  return matrix * n;
}

template <unsigned int R, unsigned int C, typename N>
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
