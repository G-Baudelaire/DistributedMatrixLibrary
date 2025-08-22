//
// Created by Germain Jones on 22/08/2025.
//

#include <Matrix.hpp>
#include <gtest/gtest.h>

#include "NaiveSubtraction.h"

class NaiveSubtractionTest : public testing::Test {
protected:
  const std::unique_ptr<NaiveSubtraction> subtraction;
  NaiveSubtractionTest() : subtraction(std::make_unique<NaiveSubtraction>()) {}
};

TEST_F(NaiveSubtractionTest, SubtractZeroValues) {
  const Matrix<int> intMatrixA({{1, 2}, {3, 4}});
  const Matrix<int> intMatrixB({{0, 0}, {0, 0}});
  EXPECT_EQ((*subtraction)(intMatrixA, intMatrixB), intMatrixA);

  const Matrix<float> floatMatrixA({{1, 2}, {3, 4}});
  const Matrix<float> floatMatrixB({{0, 0}, {0, 0}});
  EXPECT_EQ((*subtraction)(floatMatrixA, floatMatrixB), floatMatrixA);

  const Matrix<double> doubleMatrixA({{1, 2}, {3, 4}});
  const Matrix<double> doubleMatrixB({{0, 0}, {0, 0}});
  EXPECT_EQ((*subtraction)(doubleMatrixA, doubleMatrixB), doubleMatrixA);
}

TEST_F(NaiveSubtractionTest, SubtractPositiveValues) {
  const Matrix<int> intMatrixA({{1, 2}, {3, 4}});
  const Matrix<int> intMatrixB({{5, 6}, {7, 8}});
  const Matrix<int> intMatrixAMinusB({{1 - 5, 2 - 6}, {3 - 7, 4 - 8}});
  EXPECT_EQ((*subtraction)(intMatrixA, intMatrixB), intMatrixAMinusB);

  const Matrix<float> floatMatrixA({{1, 2}, {3, 4}});
  const Matrix<float> floatMatrixB({{5, 6}, {7, 8}});
  const Matrix<float> floatMatrixAMinusB({{1 - 5, 2 - 6}, {3 - 7, 4 - 8}});
  EXPECT_EQ((*subtraction)(floatMatrixA, floatMatrixB), floatMatrixAMinusB);

  const Matrix<double> doubleMatrixA({{1, 2}, {3, 4}});
  const Matrix<double> doubleMatrixB({{5, 6}, {7, 8}});
  const Matrix<double> doubleMatrixAMinusB({{1 - 5, 2 - 6}, {3 - 7, 4 - 8}});
  EXPECT_EQ((*subtraction)(doubleMatrixA, doubleMatrixB), doubleMatrixAMinusB);
}

TEST_F(NaiveSubtractionTest, SubtractNegativeValues) {
  const Matrix<int> intMatrixA({{-1, -2}, {-3, -4}});
  const Matrix<int> intMatrixB({{-5, -6}, {-7, -8}});
  const Matrix<int> intMatrixAMinusB({{(-1) - (-5), (-2) - (-6)}, {(-3) - (-7), (-4) - (-8)}});
  EXPECT_EQ((*subtraction)(intMatrixA, intMatrixB), intMatrixAMinusB);

  const Matrix<float> floatMatrixA({{-1, -2}, {-3, -4}});
  const Matrix<float> floatMatrixB({{-5, -6}, {-7, -8}});
  const Matrix<float> floatMatrixAMinusB({{(-1) - (-5), (-2) - (-6)}, {(-3) - (-7), (-4) - (-8)}});
  EXPECT_EQ((*subtraction)(floatMatrixA, floatMatrixB), floatMatrixAMinusB);

  const Matrix<double> doubleMatrixA({{-1, -2}, {-3, -4}});
  const Matrix<double> doubleMatrixB({{-5, -6}, {-7, -8}});
  const Matrix<double> doubleMatrixAMinusB({{(-1) - (-5), (-2) - (-6)}, {(-3) - (-7), (-4) - (-8)}});
  EXPECT_EQ((*subtraction)(doubleMatrixA, doubleMatrixB), doubleMatrixAMinusB);
}

TEST_F(NaiveSubtractionTest, ThrowOnDifferentRowSize) {
  const Matrix<int> matrixA(3, 1);
  const Matrix<int> matrixB(2, 1);
  EXPECT_ANY_THROW((*subtraction)(matrixA, matrixB));
}

TEST_F(NaiveSubtractionTest, ThrowOnDifferentColumnSize) {
  const Matrix<int> matrixA(2, 2);
  const Matrix<int> matrixB(2, 1);
  EXPECT_ANY_THROW((*subtraction)(matrixA, matrixB));
}
