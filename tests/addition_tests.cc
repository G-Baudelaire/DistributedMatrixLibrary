//
// Created by Germain Jones on 22/08/2025.
//

#include <gtest/gtest.h>
#include <matrix.hpp>

#include "naive_addition.hpp"

using testing::TestWithParam, testing::Values, testing::TestParamInfo;

#define ADDITION_VALIDITY_TEST(T, ADDITION_FUNCTION, ADDITION_FUNCTION_NAME)                \
  TEST(Addition_##T, ADDITION_FUNCTION_NAME##Zero) {                                        \
    const Matrix<T> matrixA({{1, 2}, {3, 4}});                                              \
    const Matrix<T> matrixB({{0, 0}, {0, 0}});                                              \
    EXPECT_EQ(ADDITION_FUNCTION(matrixA, matrixB), matrixA);                                \
  }                                                                                         \
                                                                                            \
  TEST(Addition_##T, ADDITION_FUNCTION_NAME##Positive) {                                    \
    const Matrix<T> matrixA({{1, 2}, {3, 4}});                                              \
    const Matrix<T> matrixB({{5, 6}, {7, 8}});                                              \
    const Matrix<T> matrixAPlusB({{1 + 5, 2 + 6}, {3 + 7, 4 + 8}});                         \
    EXPECT_EQ(ADDITION_FUNCTION(matrixA, matrixB), matrixAPlusB);                           \
  }                                                                                         \
                                                                                            \
  TEST(Addition_##T, ADDITION_FUNCTION_NAME##Negative) {                                    \
    const Matrix<T> matrixA({{-1, -2}, {-3, -4}});                                          \
    const Matrix<T> matrixB({{-5, -6}, {-7, -8}});                                          \
    const Matrix<T> matrixAPlusB({{(-1) + (-5), (-2) + (-6)}, {(-3) + (-7), (-4) + (-8)}}); \
    EXPECT_EQ(ADDITION_FUNCTION(matrixA, matrixB), matrixAPlusB);                           \
  }

ADDITION_VALIDITY_TEST(int, dml::naive_addition, Naive)
ADDITION_VALIDITY_TEST(float, dml::naive_addition, Naive)
ADDITION_VALIDITY_TEST(double, dml::naive_addition, Naive)
ADDITION_VALIDITY_TEST(long, dml::naive_addition, Naive)


#define MATRIX_MULTIPLICATION_ERROR_TEST(ADDITION_FUNCTION, ADDITION_FUNCTION_NAME) \
  TEST(AdditionErrorTest, ADDITION_FUNCTION_NAME##ThrowOnDifferentRowSize) {        \
    const Matrix<int> matrixA(3, 1);                                                \
    const Matrix<int> matrixB(2, 1);                                                \
    EXPECT_ANY_THROW(ADDITION_FUNCTION(matrixA, matrixB));                          \
  }                                                                                 \
                                                                                    \
  TEST(AdditionErrorTest, ADDITION_FUNCTION_NAME##ThrowOnDifferentColumnSize) {     \
    const Matrix<int> matrixA(2, 2);                                                \
    const Matrix<int> matrixB(2, 1);                                                \
    EXPECT_ANY_THROW(ADDITION_FUNCTION(matrixA, matrixB));                          \
  }

MATRIX_MULTIPLICATION_ERROR_TEST(dml::naive_addition, Naive)
