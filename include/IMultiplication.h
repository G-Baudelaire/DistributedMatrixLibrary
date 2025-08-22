//
// Created by Germain Jones on 19/08/2025.
//

#ifndef MATRIXCALCULATOR_IMULTIPLICATION_H
#define MATRIXCALCULATOR_IMULTIPLICATION_H
#include <memory>

#include "Matrix.hpp"


struct IMultiplication {
  virtual ~IMultiplication() = default;

  [[nodiscard]] virtual std::unique_ptr<IMultiplication> clone() const = 0;

  virtual Matrix<float> operator()(const float &scalar, const Matrix<float> &matrix) const = 0;

  virtual Matrix<double> operator()(const double &scalar, const Matrix<double> &matrix) const = 0;

  virtual Matrix<int> operator()(const int &scalar, const Matrix<int> &matrix) const = 0;
};


#endif // MATRIXCALCULATOR_IMULTIPLICATION_H
