//
// Created by Germain Jones on 22/08/2025.
//

#include <NaiveMatrixMultiplication.h>
#include <gtest/gtest.h>

class MatrixMultiplicationTest : public testing::Test {
protected:
  std::unique_ptr<NaiveMatrixMultiplication> matrixMultiplication;
  MatrixMultiplicationTest() : matrixMultiplication(std::make_unique<NaiveMatrixMultiplication>()) {}
};

TEST_F(MatrixMultiplicationTest, IdentityMultiplication) {
  const Matrix<int> intIdentity({{1, 0}, {0, 1}});
  const Matrix<int> intMatrixA({{1, 2}, {3, 4}});
  EXPECT_EQ((*matrixMultiplication)(intIdentity, intIdentity), intIdentity);
  EXPECT_EQ((*matrixMultiplication)(intMatrixA, intIdentity), intMatrixA);
  EXPECT_EQ((*matrixMultiplication)(intIdentity, intMatrixA), intMatrixA);

  const Matrix<float> floatIdentity({{1, 0}, {0, 1}});
  const Matrix<float> floatMatrixA({{1, 2}, {3, 4}});
  EXPECT_EQ((*matrixMultiplication)(floatIdentity, floatIdentity), floatIdentity);
  EXPECT_EQ((*matrixMultiplication)(floatMatrixA, floatIdentity), floatMatrixA);
  EXPECT_EQ((*matrixMultiplication)(floatIdentity, floatMatrixA), floatMatrixA);

  const Matrix<double> doubleIdentity({{1, 0}, {0, 1}});
  const Matrix<double> doubleMatrixA({{1, 2}, {3, 4}});
  EXPECT_EQ((*matrixMultiplication)(doubleIdentity, doubleIdentity), doubleIdentity);
  EXPECT_EQ((*matrixMultiplication)(doubleMatrixA, doubleIdentity), doubleMatrixA);
  EXPECT_EQ((*matrixMultiplication)(doubleIdentity, doubleMatrixA), doubleMatrixA);
}

TEST_F(MatrixMultiplicationTest, MatrixMultiplication) {
  const Matrix<int> intMatrixA({{1, 2}, {3, 4}});
  const Matrix<int> intMatrixB({{5, 6}, {7, 8}});
  const Matrix<int> intMatrixAB({{19, 22}, {43, 50}});
  const Matrix<int> intMatrixBA({{5, 6}, {7, 8}});
  EXPECT_EQ((*matrixMultiplication)(intIdentity, intIdentity), intIdentity);
  EXPECT_EQ((*matrixMultiplication)(intMatrixA, intIdentity), intMatrixA);
  EXPECT_EQ((*matrixMultiplication)(intIdentity, intMatrixA), intMatrixA);

  const Matrix<float> floatIdentity({{1, 0}, {0, 1}});
  const Matrix<float> floatMatrixA({{1, 2}, {3, 4}});
  EXPECT_EQ((*matrixMultiplication)(floatIdentity, floatIdentity), floatIdentity);
  EXPECT_EQ((*matrixMultiplication)(floatMatrixA, floatIdentity), floatMatrixA);
  EXPECT_EQ((*matrixMultiplication)(floatIdentity, floatMatrixA), floatMatrixA);

  const Matrix<double> doubleIdentity({{1, 0}, {0, 1}});
  const Matrix<double> doubleMatrixA({{1, 2}, {3, 4}});
  EXPECT_EQ((*matrixMultiplication)(doubleIdentity, doubleIdentity), doubleIdentity);
  EXPECT_EQ((*matrixMultiplication)(doubleMatrixA, doubleIdentity), doubleMatrixA);
  EXPECT_EQ((*matrixMultiplication)(doubleIdentity, doubleMatrixA), doubleMatrixA);
}
