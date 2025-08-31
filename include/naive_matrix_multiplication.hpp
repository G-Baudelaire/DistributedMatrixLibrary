//
// Created by Germain Jones on 31/08/2025.
//

#ifndef DMLIB_NAIVE_MATRIX_MULTIPLICATION_H
#define DMLIB_NAIVE_MATRIX_MULTIPLICATION_H

#include "matrix.hpp"
namespace dml {
  template<Numeric T>
  Matrix<T> naive_matrix_multiplication(const Matrix<T>& matrixA, const Matrix<T>& matrixB);

  namespace detail {
    template<Numeric T>
    T dot_product(
      const Matrix<T>& matrixA, const Matrix<T>& matrixB, const size_t matrixARows, const size_t matrixBColumns
    ) {
      T sum{};
      for (size_t i = 0; i < matrixA.columns(); i++) {
        sum += matrixA(matrixARows, i) * matrixB(i, matrixBColumns);
      }
      return sum;
    }
  } // namespace detail

  template<Numeric T>
  Matrix<T> naive_matrix_multiplication(const Matrix<T>& matrixA, const Matrix<T>& matrixB) {
    Matrix<T> result(matrixA.rows(), matrixB.columns(), T{});

    if (matrixA.columns() != matrixB.rows()) {
      throw std::invalid_argument("Matrix A columns must equal matrix B row.");
    }

    for (size_t row = 0; row < matrixA.rows(); row++) {
      for (size_t column = 0; column < matrixB.columns(); column++) {
        result(row, column) = detail::dot_product(matrixA, matrixB, row, column);
      }
    }
    return result;
  }
} // namespace dml

#endif // DMLIB_NAIVE_MATRIX_MULTIPLICATION_H
