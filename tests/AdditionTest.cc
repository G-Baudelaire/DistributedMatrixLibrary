//
// Created by Germain Jones on 22/08/2025.
//

#include <Matrix.hpp>
#include <NaiveAddition.h>
#include <gtest/gtest.h>

class NaiveAdditionTest : public testing::Test {
protected:
  const std::unique_ptr<NaiveAddition> addition;
  NaiveAdditionTest() : addition(std::make_unique<NaiveAddition>()) {}
};

TEST_F(NaiveAdditionTest, AddZeroValues) {
  const Matrix<int> intMatrixA({{1, 2}, {3, 4}});
  const Matrix<int> intMatrixB({{0, 0}, {0, 0}});
  EXPECT_EQ((*addition)(intMatrixA, intMatrixB), intMatrixA);

  const Matrix<float> floatMatrixA({{1, 2}, {3, 4}});
  const Matrix<float> floatMatrixB({{0, 0}, {0, 0}});
  EXPECT_EQ((*addition)(floatMatrixA, floatMatrixB), floatMatrixA);

  const Matrix<double> doubleMatrixA({{1, 2}, {3, 4}});
  const Matrix<double> doubleMatrixB({{0, 0}, {0, 0}});
  EXPECT_EQ((*addition)(doubleMatrixA, doubleMatrixB), doubleMatrixA);
}

TEST_F(NaiveAdditionTest, AddPositiveValues) {
  const Matrix<int> intMatrixA({{1, 2}, {3, 4}});
  const Matrix<int> intMatrixB({{5, 6}, {7, 8}});
  const Matrix<int> intMatrixAPlusB({{1 + 5, 2 + 6}, {3 + 7, 4 + 8}});
  EXPECT_EQ((*addition)(intMatrixA, intMatrixB), intMatrixAPlusB);

  const Matrix<float> floatMatrixA({{1, 2}, {3, 4}});
  const Matrix<float> floatMatrixB({{5, 6}, {7, 8}});
  const Matrix<float> floatMatrixAPlusB({{1 + 5, 2 + 6}, {3 + 7, 4 + 8}});
  EXPECT_EQ((*addition)(floatMatrixA, floatMatrixB), floatMatrixAPlusB);

  const Matrix<double> doubleMatrixA({{1, 2}, {3, 4}});
  const Matrix<double> doubleMatrixB({{5, 6}, {7, 8}});
  const Matrix<double> doubleMatrixAPlusB({{1 + 5, 2 + 6}, {3 + 7, 4 + 8}});
  EXPECT_EQ((*addition)(doubleMatrixA, doubleMatrixB), doubleMatrixAPlusB);
}

TEST_F(NaiveAdditionTest, AddNegativeValues) {
  const Matrix<int> intMatrixA({{-1, -2}, {-3, -4}});
  const Matrix<int> intMatrixB({{-5, -6}, {-7, -8}});
  const Matrix<int> intMatrixAPlusB({{(-1) + (-5), (-2) + (-6)}, {(-3) + (-7), (-4) + (-8)}});
  EXPECT_EQ((*addition)(intMatrixA, intMatrixB), intMatrixAPlusB);

  const Matrix<float> floatMatrixA({{-1, -2}, {-3, -4}});
  const Matrix<float> floatMatrixB({{-5, -6}, {-7, -8}});
  const Matrix<float> floatMatrixAPlusB({{(-1) + (-5), (-2) + (-6)}, {(-3) + (-7), (-4) + (-8)}});
  EXPECT_EQ((*addition)(floatMatrixA, floatMatrixB), floatMatrixAPlusB);

  const Matrix<double> doubleMatrixA({{-1, -2}, {-3, -4}});
  const Matrix<double> doubleMatrixB({{-5, -6}, {-7, -8}});
  const Matrix<double> doubleMatrixAPlusB({{(-1) + (-5), (-2) + (-6)}, {(-3) + (-7), (-4) + (-8)}});
  EXPECT_EQ((*addition)(doubleMatrixA, doubleMatrixB), doubleMatrixAPlusB);
}

TEST_F(NaiveAdditionTest, ThrowOnDifferentRowSize) {
  const Matrix<int> matrixA(3, 1);
  const Matrix<int> matrixB(2, 1);
  EXPECT_ANY_THROW((*addition)(matrixA, matrixB));
}

TEST_F(NaiveAdditionTest, ThrowOnDifferentColumnSize) {
  const Matrix<int> matrixA(2, 2);
  const Matrix<int> matrixB(2, 1);
  EXPECT_ANY_THROW((*addition)(matrixA, matrixB));
}
