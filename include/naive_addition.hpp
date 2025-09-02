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
    T *resultData = result.data();
    const T *matrixAData = matrixA.data(), *matrixBData = matrixB.data();

    for (int i = 0; i < matrixA.rows() * matrixA.columns(); i++) {
      resultData[i] = matrixAData[i] + matrixBData[i];
    }

    return result;
  }
} // namespace dml

#endif // DMLIB_NAIVE_ADDITION_H
