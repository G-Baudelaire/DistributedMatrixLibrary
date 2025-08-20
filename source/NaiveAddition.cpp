//
// Created by Germain Jones on 19/08/2025.
//
#include "../include/NaiveAddition.h"

#include <memory>

template<class T>
Matrix<T> NaiveAddition::additionImplementation(const Matrix<T> &matrixA, const Matrix<T> &matrixB) const {
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

std::unique_ptr<IAddition> NaiveAddition::clone() const {
  return std::make_unique<NaiveAddition>(*this);
}

Matrix<float> NaiveAddition::operator()(const Matrix<float> &matrixA, const Matrix<float> &matrixB) const {
  return additionImplementation(matrixA, matrixB);
}

Matrix<double> NaiveAddition::operator()(const Matrix<double> &matrixA, const Matrix<double> &matrixB) const {
  return additionImplementation(matrixA, matrixB);
}

Matrix<int> NaiveAddition::operator()(const Matrix<int> &matrixA, const Matrix<int> &matrixB) const {
  return additionImplementation(matrixA, matrixB);
}
