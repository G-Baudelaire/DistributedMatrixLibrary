//
// Created by Germain Jones on 21/08/2025.
//
#include <gtest/gtest.h>

#include "matrix.hpp"

class matrix_tests : public testing::Test {
protected:
  Matrix<int> matrixA;
  const Matrix<int> constMatrixA;
  Matrix<int> matrixB;
  Matrix<int> matrixC;
  Matrix<float> matrixD;

  matrix_tests() :
      matrixA({{1, 2}, {3, 4}}), constMatrixA({{1, 2}, {3, 4}}), matrixB({{1, 2}, {3, 4}}), matrixC({{0, 0}, {0, 0}}),
      matrixD({{0, 0}, {0, 0}}) {}
};

TEST_F(matrix_tests, EqualityByReference) { EXPECT_EQ(matrixA, matrixA); }

TEST_F(matrix_tests, EqualityByValue) { EXPECT_EQ(matrixA, matrixB); }

TEST_F(matrix_tests, InequalityByValue) { EXPECT_NE(matrixA, matrixC); }

TEST_F(matrix_tests, ReadValues) {
  EXPECT_EQ(matrixA(0, 0), 1);
  EXPECT_EQ(matrixA(0, 1), 2);
  EXPECT_EQ(matrixA(1, 0), 3);
  EXPECT_EQ(matrixA(1, 1), 4);
}

TEST_F(matrix_tests, ConstReadValues) {
  EXPECT_EQ(constMatrixA(0, 0), 1);
  EXPECT_EQ(constMatrixA(0, 1), 2);
  EXPECT_EQ(constMatrixA(1, 0), 3);
  EXPECT_EQ(constMatrixA(1, 1), 4);
}

TEST_F(matrix_tests, ReadSizeTypeOutOfRangeValues) {
  constexpr size_t zero = 0;
  constexpr size_t two = 2;
  EXPECT_ANY_THROW(matrixA(zero, two));
  EXPECT_ANY_THROW(matrixA(two, zero));
  EXPECT_ANY_THROW(matrixA(two, two));
}

TEST_F(matrix_tests, ConstReadSizeTypeOutOfRangeValues) {
  constexpr size_t zero = 0;
  constexpr size_t two = 2;
  EXPECT_ANY_THROW(constMatrixA(zero, two));
  EXPECT_ANY_THROW(constMatrixA(two, zero));
  EXPECT_ANY_THROW(constMatrixA(two, two));
}

TEST_F(matrix_tests, ReadSignedOutOfRangeValues) {
  EXPECT_ANY_THROW(matrixA(-1, 0));
  EXPECT_ANY_THROW(matrixA(0, -1));
  EXPECT_ANY_THROW(matrixA(2, 0));
  EXPECT_ANY_THROW(matrixA(0, 2));
}

TEST_F(matrix_tests, ConstReadSignedOutOfRangeValues) {
  EXPECT_ANY_THROW(constMatrixA(-1, 0));
  EXPECT_ANY_THROW(constMatrixA(0, -1));
  EXPECT_ANY_THROW(constMatrixA(2, 0));
  EXPECT_ANY_THROW(constMatrixA(0, 2));
}

TEST_F(matrix_tests, UpdateMatrixValues) {
  EXPECT_EQ(matrixA(0, 0), 1);
  EXPECT_EQ(matrixA(0, 1), 2);
  EXPECT_EQ(matrixA(1, 0), 3);
  EXPECT_EQ(matrixA(1, 1), 4);

  matrixA(0, 0) = 11;
  matrixA(1, 1) = 14;

  EXPECT_EQ(matrixA(0, 0), 11);
  EXPECT_EQ(matrixA(0, 1), 2);
  EXPECT_EQ(matrixA(1, 0), 3);
  EXPECT_EQ(matrixA(1, 1), 14);
}

TEST_F(matrix_tests, FillMethod) {
  matrixA.fill(100);
  EXPECT_EQ(matrixA, Matrix({{100, 100}, {100, 100}}));
}

TEST_F(matrix_tests, ResizeToSameSize) {
  matrixA.resize(2, 2, 1);
  EXPECT_EQ(matrixA, Matrix({{1, 1}, {1, 1}}));
}

TEST_F(matrix_tests, ResizeLarger) {
  matrixA.resize(3, 3, 1);
  EXPECT_EQ(matrixA, Matrix({{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}));
}

TEST_F(matrix_tests, ResizeSmaller) {
  matrixA.resize(1, 1, 1);
  EXPECT_EQ(matrixA, Matrix({{1}}));
}

TEST_F(matrix_tests, ZeroRowResize) { EXPECT_ANY_THROW(matrixA.resize(0, 1, 1);); }

TEST_F(matrix_tests, ZeroColumnResize) { EXPECT_ANY_THROW(matrixA.resize(1, 0, 1);); }

TEST_F(matrix_tests, NegativeRowResize) { EXPECT_ANY_THROW(matrixA.resize(-1, 1, 1);); }

TEST_F(matrix_tests, NegativeColumnResize) { EXPECT_ANY_THROW(matrixA.resize(1, -1, 1);); }
