//
// Created by Germain Jones on 22/08/2025.
//

#include <gtest/gtest.h>
#include <matrix.hpp>

#include "distributed_multiplication.hpp"
#include "naive_multiplication.hpp"

using testing::TestWithParam, testing::Values, testing::TestParamInfo;

#define MATRIX_MULTIPLICATION_VALIDITY_TEST(T, MULTIPLICATION_FUNCTION, MULTIPLICATION_FUNCTION_NAME) \
  TEST(Multiplication_##T, MULTIPLICATION_FUNCTION_NAME##Positive) {                                  \
    Matrix<T> matrix({{1, 2}, {3, 4}});                                                               \
    Matrix<T> expected({{2, 4}, {6, 8}});                                                             \
    T scalar = 2;                                                                                     \
    EXPECT_EQ(MULTIPLICATION_FUNCTION(scalar, matrix), expected);                                     \
  }                                                                                                   \
                                                                                                      \
  TEST(Multiplication_##T, MULTIPLICATION_FUNCTION_NAME##Zero) {                                      \
    Matrix<T> matrix({{1, 2, 3}, {4, 5, 6}});                                                         \
    Matrix<T> expected({{0, 0, 0}, {0, 0, 0}});                                                       \
    T scalar = 0;                                                                                     \
    EXPECT_EQ(MULTIPLICATION_FUNCTION(scalar, matrix), expected);                                     \
  }                                                                                                   \
                                                                                                      \
  TEST(Multiplication_##T, MULTIPLICATION_FUNCTION_NAME##Negative) {                                  \
    Matrix<T> matrix({{1, 2}, {3, 4}});                                                               \
    Matrix<T> expected({{-2, -4}, {-6, -8}});                                                         \
    T scalar = -2;                                                                                    \
    EXPECT_EQ(MULTIPLICATION_FUNCTION(scalar, matrix), expected);                                     \
  }

MATRIX_MULTIPLICATION_VALIDITY_TEST(int, dml::naive_multiplication, Naive)
MATRIX_MULTIPLICATION_VALIDITY_TEST(int, dml::distributed_multiplication, Distributed)
MATRIX_MULTIPLICATION_VALIDITY_TEST(float, dml::naive_multiplication, Naive)
MATRIX_MULTIPLICATION_VALIDITY_TEST(float, dml::distributed_multiplication, Distributed)
MATRIX_MULTIPLICATION_VALIDITY_TEST(double, dml::naive_multiplication, Naive)
MATRIX_MULTIPLICATION_VALIDITY_TEST(double, dml::distributed_multiplication, Distributed)
MATRIX_MULTIPLICATION_VALIDITY_TEST(long, dml::naive_multiplication, Naive)
MATRIX_MULTIPLICATION_VALIDITY_TEST(long, dml::distributed_multiplication, Distributed)
