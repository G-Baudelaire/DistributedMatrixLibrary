//
// Created by Germain Jones on 22/08/2025.
//

#include <DistributedMultiplication.h>
#include <Matrix.hpp>
#include <gtest/gtest.h>
#include "NaiveMultiplication.h"

using testing::TestWithParam, testing::Values, testing::TestParamInfo;

struct ImplCase {
  const char* name;
  std::function<std::unique_ptr<IMultiplication>()> getMultiplication;
};

class MultiplicationTest : public TestWithParam<ImplCase> {
protected:
  std::unique_ptr<IMultiplication> multiplication;
  void SetUp() override { multiplication = GetParam().getMultiplication(); }
};

static std::string NamePrinter(const TestParamInfo<ImplCase>& parameterInfo) { return parameterInfo.param.name; }

INSTANTIATE_TEST_SUITE_P(
  AllImplementations, MultiplicationTest,
  Values(
    ImplCase{"Naive", [] { return std::make_unique<NaiveMultiplication>(); }},
    ImplCase{
      "Distributed",
      [] { return std::make_unique<DistributedMultiplication>(std::getenv("DISTRIBUTED_MULTIPLICATION_WORKER"), 2); }
    }
  ),
  NamePrinter
);

TEST_P(MultiplicationTest, IntZeroMultiplication) {
  const Matrix<int> intMatrix({{1, 2, 3}, {4, 5, 6}}), intMatrixZero({{0, 0, 0}, {0, 0, 0}});
  EXPECT_EQ((*multiplication)(0, intMatrix), intMatrixZero);
}
TEST_P(MultiplicationTest, FloatZeroMultiplication) {
  const Matrix<float> floatMatrix({{1, 2}, {3, 4}}), floatMatrixZero({{0, 0}, {0, 0}});
  EXPECT_EQ((*multiplication)(0, floatMatrix), floatMatrixZero);
}

TEST_P(MultiplicationTest, DoubleZeroMultiplication) {
  const Matrix<double> doubleMatrix({{1, 2}, {3, 4}}), doubleMatrixZero({{0, 0}, {0, 0}});
  EXPECT_EQ((*multiplication)(0, doubleMatrix), doubleMatrixZero);
}

TEST_P(MultiplicationTest, IntPositiveMultiplication) {
  const Matrix<int> intMatrix({{1, 2}, {3, 4}}), intMatrixDoubled({{2, 4}, {6, 8}});
  EXPECT_EQ((*multiplication)(2, intMatrix), intMatrixDoubled);
}

TEST_P(MultiplicationTest, FloatPositiveMultiplication) {
  const Matrix<float> floatMatrix({{1, 2}, {3, 4}}), floatMatrixDoubled({{2, 4}, {6, 8}});
  EXPECT_EQ((*multiplication)(2, floatMatrix), floatMatrixDoubled);
}

TEST_P(MultiplicationTest, DoublePositiveMultiplication) {
  const Matrix<double> doubleMatrix({{1, 2}, {3, 4}}), doubleMatrixDoubled({{2, 4}, {6, 8}});
  EXPECT_EQ((*multiplication)(2, doubleMatrix), doubleMatrixDoubled);
}

TEST_P(MultiplicationTest, IntNegativeMultiplication) {
  const Matrix<int> intMatrix({{1, 2}, {3, 4}}), intMatrixTimesMinusTwo({{-2, -4}, {-6, -8}});
  EXPECT_EQ((*multiplication)(-2, intMatrix), intMatrixTimesMinusTwo);
}

TEST_P(MultiplicationTest, FloatNegativeMultiplication) {
  const Matrix<float> floatMatrix({{1, 2}, {3, 4}}), floatMatrixTimesMinusTwo({{-2, -4}, {-6, -8}});
  EXPECT_EQ((*multiplication)(-2, floatMatrix), floatMatrixTimesMinusTwo);
}

TEST_P(MultiplicationTest, NegativeMultiplication) {
  const Matrix<double> doubleMatrix({{1, 2}, {3, 4}}), TimesMinusTwoMatrixTimesMinusTwo({{-2, -4}, {-6, -8}});
  EXPECT_EQ((*multiplication)(-2, doubleMatrix), TimesMinusTwoMatrixTimesMinusTwo);
}
