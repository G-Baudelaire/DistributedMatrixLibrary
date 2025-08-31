//
// Created by Germain Jones on 31/08/2025.
//

#ifndef DMLIB_NAIVE_MULTIPLICATION_H
#define DMLIB_NAIVE_MULTIPLICATION_H

#include "matrix.hpp"

namespace dml {
  template<Numeric T>
  Matrix<T> naive_multiplication(const T& scalar, const Matrix<T>& matrix) {
    Matrix<T> result(matrix.rows(), matrix.columns(), 0);

    for (size_t row = 0; row < matrix.rows(); row++) {
      for (size_t column = 0; column < matrix.rows(); column++) {
        result(row, column) = scalar * matrix(row, column);
      }
    }

    return result;
  }
} // namespace dml

#endif // DMLIB_NAIVE_MULTIPLICATION_H
