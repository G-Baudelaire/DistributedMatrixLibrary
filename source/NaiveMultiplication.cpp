//
// Created by Germain Jones on 19/08/2025.
//

#include "../include/NaiveMultiplication.h"


template<class T>
Matrix<T> NaiveMultiplication::MultiplicationImplementation(const T &scalar, const Matrix<T> &matrix) const {
  Matrix<T> result(matrix.rows(), matrix.columns(), 0);

  for (size_t row = 0; row < matrix.rows(); row++) {
    for (size_t column = 0; column < matrix.rows(); column++) {
      result(row, column) = scalar * matrix(row, column);
    }
  }

  return result;
}

std::unique_ptr<IMultiplication> NaiveMultiplication::clone() const {
  return std::make_unique<NaiveMultiplication>(*this);
}

Matrix<float> NaiveMultiplication::operator()(const float &scalar, const Matrix<float> &matrix) const {
  return MultiplicationImplementation(scalar, matrix);
}

Matrix<double> NaiveMultiplication::operator()(const double &scalar, const Matrix<double> &matrix) const {
  return MultiplicationImplementation(scalar, matrix);
}

Matrix<int> NaiveMultiplication::operator()(const int &scalar, const Matrix<int> &matrix) const {
  return MultiplicationImplementation(scalar, matrix);
}
