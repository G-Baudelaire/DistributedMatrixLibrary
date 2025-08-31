//
// Created by Germain Jones on 31/08/2025.
//

#ifndef DMLIB_NAIVE_ADDITION_H
#define DMLIB_NAIVE_ADDITION_H

#include "matrix.hpp"

namespace dml {
  template<Numeric T>
  Matrix<T> naive_addition(const Matrix<T> &matrixA, const Matrix<T> &matrixB) {
    if (matrixA.rows() != matrixB.rows() || matrixA.columns() != matrixB.columns()) {
      throw std::invalid_argument("Incorrect matrix dimensions.");
    }

    Matrix<T> result(matrixA.rows(), matrixB.columns(), T{});

    for (size_t row = 0; row < matrixA.rows(); row++) {
      for (size_t column = 0; column < matrixA.columns(); column++) {
        result(row, column) = matrixA(row, column) + matrixB(row, column);
      }
    }

    return result;
  }
} // namespace dml

#endif // DMLIB_NAIVE_ADDITION_H
