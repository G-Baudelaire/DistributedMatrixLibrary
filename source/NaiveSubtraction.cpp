//
// Created by Germain Jones on 19/08/2025.
//

#include "../include/NaiveSubtraction.h"

template<class T>
Matrix<T> NaiveSubtraction::subtractionImplementation(const Matrix<T> &matrixA, const Matrix<T> &matrixB) const {
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

std::unique_ptr<ISubtraction> NaiveSubtraction::clone() const { return std::make_unique<NaiveSubtraction>(*this); }

Matrix<float> NaiveSubtraction::operator()(const Matrix<float> &matrixA, const Matrix<float> &matrixB) const {
  return subtractionImplementation(matrixA, matrixB);
}

Matrix<double> NaiveSubtraction::operator()(const Matrix<double> &matrixA, const Matrix<double> &matrixB) const {
  return subtractionImplementation(matrixA, matrixB);
}

Matrix<int> NaiveSubtraction::operator()(const Matrix<int> &matrixA, const Matrix<int> &matrixB) const {
  return subtractionImplementation(matrixA, matrixB);
}
