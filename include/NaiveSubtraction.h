//
// Created by Germain Jones on 19/08/2025.
//

#ifndef MATRIXCALCULATOR_SUBTRACTION_H
#define MATRIXCALCULATOR_SUBTRACTION_H
#include "ISubtraction.h"


class NaiveSubtraction final : public ISubtraction {
  template<class T>
  Matrix<T> subtractionImplementation(const Matrix<T> &matrixA, const Matrix<T> &matrixB) const;

public:
  [[nodiscard]] std::unique_ptr<ISubtraction> clone() const override;

  Matrix<float> operator()(const Matrix<float> &matrixA, const Matrix<float> &matrixB) const override;

  Matrix<double> operator()(const Matrix<double> &matrixA, const Matrix<double> &matrixB) const override;

  Matrix<int> operator()(const Matrix<int> &matrixA, const Matrix<int> &matrixB) const override;
};


#endif //MATRIXCALCULATOR_SUBTRACTION_H
