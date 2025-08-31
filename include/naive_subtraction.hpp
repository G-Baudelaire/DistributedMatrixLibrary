//
// Created by Germain Jones on 31/08/2025.
//

#ifndef DMLIB_NAIVE_SUBTRACTION_H
#define DMLIB_NAIVE_SUBTRACTION_H
#include "matrix.hpp"

namespace dml {
  template<Numeric T>
  Matrix<T> naive_subtraction(const Matrix<T> &matrixA, const Matrix<T> &matrixB) {
    if (matrixA.rows() != matrixB.rows() || matrixA.columns() != matrixB.columns()) {
      throw std::invalid_argument("Incorrect matrix dimension.");
    }

    Matrix<T> result(matrixA.rows(), matrixA.columns());

    for (size_t row = 0; row < matrixA.rows(); row++) {
      for (size_t column = 0; column < matrixA.columns(); column++) {
        result(row, column) = matrixA(row, column) - matrixB(row, column);
      }
    }

    return result;
  }
} // namespace dml

#endif // DMLIB_NAIVE_SUBTRACTION_H
