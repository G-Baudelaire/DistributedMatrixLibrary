//
// Created by Germain Jones on 19/08/2025.
//

#include "../include/NaiveMatrixMultiplication.h"


template<class T>
Matrix<T> NaiveMatrixMultiplication::MatrixMultiplicationImplementation(
  const Matrix<T>& matrixA,
  const Matrix<T>& matrixB) const {
  Matrix<T> result(matrixA.rows(), matrixB.columns(), T{});

  if (matrixA.columns() != matrixB.rows()) { throw std::invalid_argument("Matrix A columns must equal matrix B row."); }

  for (size_t row = 0; row < matrixA.rows(); row++) {
    for (size_t column = 0; column < matrixA.columns(); column++) {
      result(row, column) = dot_product();
    }
  }
  return result;
}

template<class T>
T DotProduct(Matrix<T>& matrixA, Matrix<T>& matrixB, size_t matrixARow, size_t matrixBColumn) {
  T sum{};

  for (size_t i = 0; i < matrixA.columns(); i++) {
    sum += matrixA(matrixARow, i) * matrixB(i, matrixBColumn);
  }

  return sum;
}

std::unique_ptr<IMatrixMultiplication> NaiveMatrixMultiplication::clone() const {
  return std::make_unique<NaiveMatrixMultiplication>(*this);
}

Matrix<float> NaiveMatrixMultiplication::operator()(const Matrix<float>& matrixA, const Matrix<float>& matrixB) const {
  return MatrixMultiplicationImplementation(matrixA, matrixB);
}

Matrix<double> NaiveMatrixMultiplication::operator()(
  const Matrix<double>& matrixA,
  const Matrix<double>& matrixB) const {
  return MatrixMultiplicationImplementation(matrixA, matrixB);
}

Matrix<int> NaiveMatrixMultiplication::operator()(const Matrix<int>& matrixA, const Matrix<int>& matrixB) const {
  return MatrixMultiplicationImplementation(matrixA, matrixB);
}
