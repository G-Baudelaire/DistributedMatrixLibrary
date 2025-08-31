//
// Created by Germain Jones on 22/08/2025.
//

#include <gtest/gtest.h>
#include <matrix.hpp>

#include "distributed_subtraction.h"
#include "naive_subtraction.hpp"

using testing::TestWithParam, testing::Values, testing::TestParamInfo;

#define SUBTRACTION_VALIDITY_TEST(T, SUBTRACTION_FUNCTION, SUBTRACTION_FUNCTION_NAME)        \
  TEST(Subtraction_##T, SUBTRACTION_FUNCTION_NAME##Zero) {                                   \
    const Matrix<T> matrixA({{1, 2}, {3, 4}});                                               \
    const Matrix<T> matrixB({{0, 0}, {0, 0}});                                               \
    EXPECT_EQ(SUBTRACTION_FUNCTION(matrixA, matrixB), matrixA);                              \
  }                                                                                          \
                                                                                             \
  TEST(Subtraction_##T, SUBTRACTION_FUNCTION_NAME##Positive) {                               \
    const Matrix<T> matrixA({{1, 2}, {3, 4}});                                               \
    const Matrix<T> matrixB({{5, 6}, {7, 8}});                                               \
    const Matrix<T> matrixAMinusB({{1 - 5, 2 - 6}, {3 - 7, 4 - 8}});                         \
    EXPECT_EQ(SUBTRACTION_FUNCTION(matrixA, matrixB), matrixAMinusB);                        \
  }                                                                                          \
                                                                                             \
  TEST(Subtraction_##T, SUBTRACTION_FUNCTION_NAME##Negative) {                               \
    const Matrix<T> matrixA({{-1, -2}, {-3, -4}});                                           \
    const Matrix<T> matrixB({{-5, -6}, {-7, -8}});                                           \
    const Matrix<T> matrixAMinusB({{(-1) - (-5), (-2) - (-6)}, {(-3) - (-7), (-4) - (-8)}}); \
    EXPECT_EQ(SUBTRACTION_FUNCTION(matrixA, matrixB), matrixAMinusB);                        \
  }

SUBTRACTION_VALIDITY_TEST(int, dml::naive_subtraction, Naive)
SUBTRACTION_VALIDITY_TEST(float, dml::naive_subtraction, Naive)
SUBTRACTION_VALIDITY_TEST(double, dml::naive_subtraction, Naive)
SUBTRACTION_VALIDITY_TEST(long, dml::naive_subtraction, Naive)

SUBTRACTION_VALIDITY_TEST(int, dml::distributed_subtraction, Distributed)
SUBTRACTION_VALIDITY_TEST(float, dml::distributed_subtraction, Distributed)
SUBTRACTION_VALIDITY_TEST(double, dml::distributed_subtraction, Distributed)
SUBTRACTION_VALIDITY_TEST(long, dml::distributed_subtraction, Distributed)

#define SUBTRACTION_ERROR_TEST(SUBTRACTION_FUNCTION, SUBTRACTION_FUNCTION_NAME)       \
  TEST(SubtractionErrorTest, SUBTRACTION_FUNCTION_NAME##ThrowOnDifferentRowSize) {    \
    const Matrix<int> matrixA(3, 1);                                                  \
    const Matrix<int> matrixB(2, 1);                                                  \
    EXPECT_ANY_THROW(SUBTRACTION_FUNCTION(matrixA, matrixB));                         \
  }                                                                                   \
                                                                                      \
  TEST(SubtractionErrorTest, SUBTRACTION_FUNCTION_NAME##ThrowOnDifferentColumnSize) { \
    const Matrix<int> matrixA(2, 2);                                                  \
    const Matrix<int> matrixB(2, 1);                                                  \
    EXPECT_ANY_THROW(SUBTRACTION_FUNCTION(matrixA, matrixB));                         \
  }

SUBTRACTION_ERROR_TEST(dml::naive_subtraction, Naive)
SUBTRACTION_ERROR_TEST(dml::distributed_subtraction, Distributed)
