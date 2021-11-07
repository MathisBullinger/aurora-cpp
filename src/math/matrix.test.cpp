#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "./matrix.hpp"
#include "./vector.hpp"

#include <chrono>

using namespace aur;
using ::testing::ElementsAre;

TEST(Matrix, construct) {
  ASSERT_THAT((Matrix<2,2,int>{}).values, ElementsAre(1,0,0,1));

  ASSERT_THAT((Matrix<2,2,int>{0}).values, ElementsAre(0,0,0,0));

  ASSERT_THAT((Matrix<2,2,int>{1,2,3,4}).values, ElementsAre(1,3,2,4));
}

TEST(Matrix, scalar_multiplication) {
  ASSERT_THAT((2.5 * Matrix<2, 2, int>{1,2,3,4}).values, ElementsAre(2.5, 7.5, 5, 10));
  ASSERT_THAT((Matrix<2, 2, int>{1,2,3,4} * 2.5).values, ElementsAre(2.5, 7.5, 5, 10));
}

TEST(Matrix, vector_multiplication) {
  Matrix<3, 3, int> transform{
    1, 0, 2,
    0, 1, 3,
    0, 0, 1
  };
  Vector<3, int> vec{1, 1, 1};
  auto res = transform * vec;

  ASSERT_THAT( res.values, ElementsAre(3, 4, 1) );
}

TEST(Matrix, matrix_multiplication) {
  Matrix<3, 2, int> a{
    1, 3,
    2, 1,
    4, 2
  };
  
  Matrix<2, 3, int> b {
    2, 1, 4,
    3, 2, 5
  };

  Matrix<3, 3, int> expected{
    11, 7, 19,
     7, 4, 13,
    14, 8, 26
  };
  
  EXPECT_TRUE( 0 == std::memcmp( (a * b).values, expected.values, 3*3*sizeof(int) ) );
}

TEST(Matrix, determinant) {
  ASSERT_EQ((Matrix<2, 2>{
    1, 2,
    3, 4
  }).determinant(), -2);

  ASSERT_EQ((Matrix<3, 3>{
     1,  2, -1,
     3,  0,  1,
    -5,  4,  2
  }).determinant(), -38);

  ASSERT_EQ((Matrix<4, 4>{
     1, -2,  9,  6,
     2,  3, -4,  2,
     4,  0,  1,  2,
     5,  1,  0,  8
  }).determinant(), -338);
}

TEST(Matrix, transpose) {
  Matrix<3, 2, int> a{
    1, 2,
    3, 4,
    5, 6
  };

  Matrix<2, 3, int> b{
    1, 3, 5,
    2, 4, 6
  };

  EXPECT_TRUE( 0 == std::memcmp( a.transpose().values, b.values, 3*2*sizeof(int) ) );
}

TEST(Matrix, inverse) {
  Matrix<3, 3> mat3{
    1,  0, -3,
    2, -2,  1,
    0, -1,  3
  };

  Matrix<3, 3> inverse3{
    -5, 3, -6,
    -6, 3, -7,
    -2, 1, -2
  };

  EXPECT_TRUE( 0 == std::memcmp( mat3.inverse().values, inverse3.values, 3*3*sizeof(float) ) );

  Matrix<4, 4> mat4{
    0, -2, -4, 0,
    3, 2, -3, 2,
    4, 4, 3, -2,
    -1, 0, 2, 0
  };

  Matrix<4, 4> inverse4{
    3, 1, 1, 6,
    -3.5, -1, -1, -7,
    1.5, 0.5, 0.5, 3.5,
    1.25, 0.75, 0.25, 3.25
  };

  EXPECT_TRUE( 0 == std::memcmp( mat4.inverse().values, inverse4.values, 3*3*sizeof(float) ) );
}
