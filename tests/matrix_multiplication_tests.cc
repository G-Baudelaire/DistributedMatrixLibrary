//
// Created by Germain Jones on 22/08/2025.
//

#include <gtest/gtest.h>

#include "naive_matrix_multiplication.hpp"

#define MATRIX_MULTIPLICATION_VALIDITY_TEST(T, MATRIX_MULTIPLICATION_FUNCTION, MATRIX_MULTIPLICATION_FUNCTION_NAME) \
                                                                                                                    \
  TEST(MatrixMultiplication_##T, MATRIX_MULTIPLICATION_FUNCTION_NAME##IdentityMultiplication) {                     \
    const Matrix<T> identity({{1, 0}, {0, 1}});                                                                     \
    const Matrix<T> matrixA({{1, 2}, {3, 4}});                                                                      \
    EXPECT_EQ(MATRIX_MULTIPLICATION_FUNCTION(identity, identity), identity);                                        \
    EXPECT_EQ(MATRIX_MULTIPLICATION_FUNCTION(matrixA, identity), matrixA);                                          \
    EXPECT_EQ(MATRIX_MULTIPLICATION_FUNCTION(identity, matrixA), matrixA);                                          \
  }                                                                                                                 \
  TEST(MatrixMultiplication_##T, MATRIX_MULTIPLICATION_FUNCTION_NAME##MatrixMultiplication) {                       \
    const Matrix<T> matrixA({{1, 2}, {3, 4}});                                                                      \
    const Matrix<T> matrixB({{5, 6}, {7, 8}});                                                                      \
    const Matrix<T> matrixAB({{19, 22}, {43, 50}});                                                                 \
    const Matrix<T> matrixBA({{23, 34}, {31, 46}});                                                                 \
    EXPECT_EQ(MATRIX_MULTIPLICATION_FUNCTION(matrixA, matrixB), matrixAB);                                          \
    EXPECT_EQ(MATRIX_MULTIPLICATION_FUNCTION(matrixB, matrixA), matrixBA);                                          \
  }

MATRIX_MULTIPLICATION_VALIDITY_TEST(int, dml::naive_matrix_multiplication, Naive)
MATRIX_MULTIPLICATION_VALIDITY_TEST(float, dml::naive_matrix_multiplication, Naive)
MATRIX_MULTIPLICATION_VALIDITY_TEST(double, dml::naive_matrix_multiplication, Naive)
MATRIX_MULTIPLICATION_VALIDITY_TEST(long, dml::naive_matrix_multiplication, Naive)

#define MATRIX_MULTIPLICATION_ERROR_TEST(MATRIX_MULTIPLICATION_FUNCTION, MATRIX_MULTIPLICATION_FUNCTION_NAME) \
  TEST(MatrixMultiplicationErrorTest, MATRIX_MULTIPLICATION_FUNCTION_NAME##ThrowOnRowColumnMismatch) {        \
    const Matrix<int> matrixA(1, 5);                                                                          \
    const Matrix<int> matrixB(4, 1);                                                                          \
    EXPECT_ANY_THROW(MATRIX_MULTIPLICATION_FUNCTION(matrixA, matrixB));                                       \
  }                                                                                                           \
                                                                                                              \
  TEST(MatrixMultiplicationErrorTest, ValidateOutputMatrixDimension) {                                        \
    Matrix<int> matrixA(1, 5);                                                                                \
    Matrix<int> matrixB(5, 1);                                                                                \
    Matrix<int> matrixAB = MATRIX_MULTIPLICATION_FUNCTION(matrixA, matrixB);                                  \
    EXPECT_EQ(matrixAB.rows(), 1);                                                                            \
    EXPECT_EQ(matrixAB.columns(), 1);                                                                         \
                                                                                                              \
    Matrix<int> matrixC(2, 5);                                                                                \
    Matrix<int> matrixD(5, 3);                                                                                \
    Matrix<int> matrixCD = MATRIX_MULTIPLICATION_FUNCTION(matrixC, matrixD);                                  \
    EXPECT_EQ(matrixCD.rows(), 2);                                                                            \
    EXPECT_EQ(matrixCD.columns(), 3);                                                                         \
  }

MATRIX_MULTIPLICATION_ERROR_TEST(dml::naive_matrix_multiplication, Naive)
