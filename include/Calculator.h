//
// Created by Germain Jones on 19/08/2025.
//

#ifndef DMLIB_CALCULATOR_H
#define DMLIB_CALCULATOR_H

#include "IAddition.h"
#include "IMatrixMultiplication.h"
#include "IMultiplication.h"
#include "ISubtraction.h"
#include "Matrix.hpp"

class Calculator {
  const std::unique_ptr<IMultiplication> multiplication_;
  const std::unique_ptr<IMatrixMultiplication> matrixMultiplication_;
  const std::unique_ptr<IAddition> addition_;
  const std::unique_ptr<ISubtraction> subtraction_;

public:
  Calculator(
      std::unique_ptr<IMultiplication> multiplication,
      std::unique_ptr<IMatrixMultiplication> matrixMultiplication,
      std::unique_ptr<IAddition> addition,
      std::unique_ptr<ISubtraction> subtraction) :
      multiplication_(std::move(multiplication)), matrixMultiplication_(std::move(matrixMultiplication)),
      addition_(std::move(addition)), subtraction_(std::move(subtraction)) {}

  template<class T>
  Matrix<T> Multiply(const T &scalar, const Matrix<T> &matrix) {
    return (*multiplication_)(scalar, matrix);
  }

  template<class T>
  Matrix<T> Multiply(const Matrix<T> &matrix, const T &scalar) {
    return (*multiplication_)(scalar, matrix);
  }

  template<class T>
  Matrix<T> Add(const Matrix<T> &matrixA, const Matrix<T> &matrixB) {
    return (*addition_)(matrixA, matrixB);
  }

  template<class T>
  Matrix<T> Subtract(const Matrix<T> &matrixA, const Matrix<T> &matrixB) {
    return (*subtraction_)(matrixA, matrixB);
  }

  template<class T>
  Matrix<T> MatrixMultiply(const Matrix<T> &matrixA, const Matrix<T> &matrixB) {
    return (*matrixMultiplication_)(matrixA, matrixB);
  }
};

#endif // DMLIB_CALCULATOR_H
