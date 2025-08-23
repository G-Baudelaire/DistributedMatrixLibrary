//
// Created by Germain Jones on 19/08/2025.
//

#ifndef DMLIB_NAIVEMULTIPLICATION_H
#define DMLIB_NAIVEMULTIPLICATION_H
#include "IMultiplication.h"


class NaiveMultiplication final : public IMultiplication {
  template<class T>
  Matrix<T> MultiplicationImplementation(const T &scalar, const Matrix<T> &matrix) const;

public:
  [[nodiscard]] std::unique_ptr<IMultiplication> clone() const override;

  Matrix<float> operator()(const float &scalar, const Matrix<float> &matrix) const override;

  Matrix<double> operator()(const double &scalar, const Matrix<double> &matrix) const override;


  Matrix<int> operator()(const int &scalar, const Matrix<int> &matrix) const override;
};


#endif // DMLIB_NAIVEMULTIPLICATION_H
