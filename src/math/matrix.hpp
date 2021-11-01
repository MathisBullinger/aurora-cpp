#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cmath>

template <const unsigned int rows, const unsigned int columns, typename T = double>
class Matrix {
  public:
    Matrix() {
      for (unsigned char i = 0; i < std::min(rows, columns); i++)
        values[i * columns + i] = 1;
    }
    
    template <typename... U>
    Matrix(U... init) : values{init...} {}

    T values[rows * columns] = {0};

    void set(unsigned int row, unsigned int column, T value) {
      values[row * columns + column] = value;
    }

    template <const unsigned int bcols>
    Matrix<rows, bcols, T> operator*(const Matrix<columns, bcols, T>& rhs) {
      Matrix<rows, bcols, T> result({0});

      for (unsigned int r = 0; r < rows; r++) {
        for (unsigned int c = 0; c < bcols; c++) {
          for (unsigned int i = 0; i < columns; i++)
            result.values[r * bcols + c]  += values[r * columns + i] * rhs.values[i * bcols + c];
        }
      }

      return result;
    }

    template <const unsigned int R, const unsigned int C, typename N>
    friend std::ostream& operator<<( std::ostream&, const Matrix<R, C, N>& );
};

template <typename T>
static std::unique_ptr<Matrix<4, 4, T>> translation(T x, T y, T z) {
  std::unique_ptr<Matrix<4, 4, T>> result(new Matrix<4, 4, T>());
  result->set(0, 3, x);
  result->set(1, 3, y);
  result->set(2, 3, z);
  return result;
}

static std::unique_ptr<Matrix<4, 4, float>> perspective(float aspect, float fov, float near, float far) {
  return std::unique_ptr<Matrix<4, 4, float>>(new Matrix<4, 4, float>({
    1 / aspect * tan(fov / 2), 0.f, 0.f, 0.f,
    0.f, 1 / tan(fov / 2), 0.f, 0.f,
    0.f, 0.f, -(far + near) / (far - near), -(2 * far * near) / (far - near),
    0.f, 0.f, -1.f, 0.f,
  }));
}

template <const unsigned int R, const unsigned int C, typename N>
std::ostream& operator<<(std::ostream& os, const Matrix<R, C, N>& target) {
  os << R << "x" << C << " Matrix <" << typeid(N).name() << ">";

  unsigned int maxLength = 0;
  std::vector<std::vector<std::string>> rows;

  for (auto r = 0; r < R; r++) {
    std::vector<std::string> row;
    for (auto c = 0; c < C; c++) {
      std::string str = std::to_string(target.values[r * C + c]);
      str.erase(str.find_last_not_of('0') + 1, std::string::npos);
      if (str.ends_with('.')) str.pop_back();
      if (str.size() > maxLength) maxLength = str.size();
      row.push_back(std::to_string(target.values[r * C + c]));
    }
    rows.push_back(row);
  }

  for (auto row : rows) {
    os << "\n";
    for (auto v : row) os << "  " << v.substr(0, maxLength);
  }
  
  return os;
}
