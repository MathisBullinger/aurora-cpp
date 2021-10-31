#pragma once

template <const unsigned int rows, const unsigned int columns, typename T = double>
class Matrix {
  public:
    Matrix() {
      for (unsigned char i = 0; i < std::min(rows, columns); i++)
        data[i * columns + i] = 1;
    }
  
    template <const unsigned int R, const unsigned int C, typename N>
    friend std::ostream& operator<<( std::ostream&, const Matrix<R, C, N>& );
  
  private:
    T data[rows * columns] = {0};
};

template <const unsigned int R, const unsigned int C, typename N>
std::ostream& operator<<(std::ostream& os, const Matrix<R, C, N>& target) {
  os << R << "x" << C << " Matrix (" << typeid(N).name() << ")";

  for (auto r = 0; r < R; r++) {
    os << "\n";
    for (auto c = 0; c < C; c++) {
      os << " " << target.data[r * C + c];
    }
  }
  
  return os;
}
