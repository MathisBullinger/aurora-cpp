#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "./matrix.hpp"

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
}
