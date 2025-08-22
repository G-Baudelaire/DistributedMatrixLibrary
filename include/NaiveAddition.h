//
// Created by Germain Jones on 19/08/2025.
//

#ifndef MATRIXCALCULATOR_ADDNAIVE_H
#define MATRIXCALCULATOR_ADDNAIVE_H

#include "IAddition.h"

class NaiveAddition final : public IAddition {
  template<class T>
  Matrix<T> additionImplementation(const Matrix<T> &matrixA, const Matrix<T> &matrixB) const;

public:
  Matrix<float> operator()(const Matrix<float> &matrixA, const Matrix<float> &matrixB) const override;

  Matrix<double> operator()(const Matrix<double> &matrixA, const Matrix<double> &matrixB) const override;

  Matrix<int> operator()(const Matrix<int> &matrixA, const Matrix<int> &matrixB) const override;

  [[nodiscard]] std::unique_ptr<IAddition> clone() const override;
};


#endif // MATRIXCALCULATOR_ADDNAIVE_H
