//
// Created by Germain Jones on 22/08/2025.
//

#include <Matrix.hpp>
#include <gtest/gtest.h>

#include "NaiveMultiplication.h"

class NaiveMultiplicationTest : public testing::Test {
protected:
  const std::unique_ptr<NaiveMultiplication> multiplication;
  NaiveMultiplicationTest() : multiplication(std::make_unique<NaiveMultiplication>()) {}
};

TEST_F(NaiveMultiplicationTest, ZeroMultiplication) {
  const Matrix<int> intMatrix({{1, 2}, {3, 4}});
  const Matrix<int> intMatrixZero({{0, 0}, {0, 0}});
  const Matrix<float> floatMatrix({{1, 2}, {3, 4}});
  const Matrix<float> floatMatrixZero({{0, 0}, {0, 0}});
  const Matrix<double> doubleMatrix({{1, 2}, {3, 4}});
  const Matrix<double> doubleMatrixZero({{0, 0}, {0, 0}});
  EXPECT_EQ((*multiplication)(0, intMatrix), intMatrixZero);
  EXPECT_EQ((*multiplication)(0, floatMatrix), floatMatrixZero);
  EXPECT_EQ((*multiplication)(0, doubleMatrix), doubleMatrixZero);
}

TEST_F(NaiveMultiplicationTest, PositiveMultiplication) {
  const Matrix<int> intMatrix({{1, 2}, {3, 4}});
  const Matrix<int> intMatrixDoubled({{2, 4}, {6, 8}});
  const Matrix<float> floatMatrix({{1, 2}, {3, 4}});
  const Matrix<float> floatMatrixDoubled({{2, 4}, {6, 8}});
  const Matrix<double> doubleMatrix({{1, 2}, {3, 4}});
  const Matrix<double> doubleMatrixDoubled({{2, 4}, {6, 8}});
  EXPECT_EQ((*multiplication)(2, intMatrix), intMatrixDoubled);
  EXPECT_EQ((*multiplication)(2, floatMatrix), floatMatrixDoubled);
  EXPECT_EQ((*multiplication)(2, doubleMatrix), doubleMatrixDoubled);
}

TEST_F(NaiveMultiplicationTest, NegativeMultiplication) {
  const Matrix<int> intMatrix({{1, 2}, {3, 4}});
  const Matrix<int> intMatrixTimesMinusTwo({{-2, -4}, {-6, -8}});
  const Matrix<float> floatMatrix({{1, 2}, {3, 4}});
  const Matrix<float> floatMatrixTimesMinusTwo({{-2, -4}, {-6, -8}});
  const Matrix<double> TimesMinusTwoMatrix({{1, 2}, {3, 4}});
  const Matrix<double> TimesMinusTwoMatrixTimesMinusTwo({{-2, -4}, {-6, -8}});
  EXPECT_EQ((*multiplication)(-2, intMatrix), intMatrixTimesMinusTwo);
  EXPECT_EQ((*multiplication)(-2, floatMatrix), floatMatrixTimesMinusTwo);
  EXPECT_EQ((*multiplication)(-2, TimesMinusTwoMatrix), TimesMinusTwoMatrixTimesMinusTwo);
}
