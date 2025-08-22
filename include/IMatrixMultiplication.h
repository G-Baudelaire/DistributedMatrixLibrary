//
// Created by Germain Jones on 19/08/2025.
//

#ifndef MATRIXCALCULATOR_IMATRIXMULTIPLICATION_H
#define MATRIXCALCULATOR_IMATRIXMULTIPLICATION_H

#include <memory>

#include "Matrix.hpp"

struct IMatrixMultiplication {
  virtual ~IMatrixMultiplication() = default;

  [[nodiscard]] virtual std::unique_ptr<IMatrixMultiplication> clone() const = 0;

  virtual Matrix<float> operator()(const Matrix<float> &matrixA, const Matrix<float> &matrixB) const = 0;

  virtual Matrix<double> operator()(const Matrix<double> &matrixA, const Matrix<double> &matrixB) const = 0;

  virtual Matrix<int> operator()(const Matrix<int> &matrixA, const Matrix<int> &matrixB) const = 0;
};
#endif // MATRIXCALCULATOR_IMATRIXMULTIPLICATION_H
