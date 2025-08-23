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
  const Matrix<int> intMatrixBA({{23, 34}, {31, 46}});
  EXPECT_EQ((*matrixMultiplication)(intMatrixA, intMatrixB), intMatrixAB);
  EXPECT_EQ((*matrixMultiplication)(intMatrixB, intMatrixA), intMatrixBA);

  const Matrix<float> floatMatrixA({{1, 2}, {3, 4}});
  const Matrix<float> floatMatrixB({{5, 6}, {7, 8}});
  const Matrix<float> floatMatrixAB({{19, 22}, {43, 50}});
  const Matrix<float> floatMatrixBA({{23, 34}, {31, 46}});
  EXPECT_EQ((*matrixMultiplication)(floatMatrixA, floatMatrixB), floatMatrixAB);
  EXPECT_EQ((*matrixMultiplication)(floatMatrixB, floatMatrixA), floatMatrixBA);

  const Matrix<double> doubleMatrixA({{1, 2}, {3, 4}});
  const Matrix<double> doubleMatrixB({{5, 6}, {7, 8}});
  const Matrix<double> doubleMatrixAB({{19, 22}, {43, 50}});
  const Matrix<double> doubleMatrixBA({{23, 34}, {31, 46}});
  EXPECT_EQ((*matrixMultiplication)(doubleMatrixA, doubleMatrixB), doubleMatrixAB);
  EXPECT_EQ((*matrixMultiplication)(doubleMatrixB, doubleMatrixA), doubleMatrixBA);
}

TEST_F(MatrixMultiplicationTest, ThrowOnRowColumnMismatch) {
  const Matrix<int> matrixA(1, 5);
  const Matrix<int> matrixB(4, 1);
  EXPECT_ANY_THROW((*matrixMultiplication)(matrixA, matrixB));
}

TEST_F(MatrixMultiplicationTest, ValidateOutputMatrixDimension) {
  Matrix<int> matrixA(1, 5);
  Matrix<int> matrixB(5, 1);
  Matrix<int> matrixAB = (*matrixMultiplication)(matrixA, matrixB);
  EXPECT_EQ(matrixAB.rows(), 1);
  EXPECT_EQ(matrixAB.columns(), 1);

  Matrix<int> matrixC(2, 5);
  Matrix<int> matrixD(5, 3);
  Matrix<int> matrixCD = (*matrixMultiplication)(matrixC, matrixD);
  EXPECT_EQ(matrixCD.rows(), 2);
  EXPECT_EQ(matrixCD.columns(), 3);
}
