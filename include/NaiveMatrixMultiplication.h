//
// Created by Germain Jones on 19/08/2025.
//

#ifndef DMLIB_NAIVEMATRIXMULTIPLICATION_H
#define DMLIB_NAIVEMATRIXMULTIPLICATION_H

#include "IMatrixMultiplication.h"

class NaiveMatrixMultiplication final : public IMatrixMultiplication {
  template<class T>
  T DotProduct(const Matrix<T> &matrixA, const Matrix<T> &matrixB, size_t matrixARow, size_t matrixBColumn) const;

  template<class T>
  Matrix<T> MatrixMultiplicationImplementation(const Matrix<T> &matrixA, const Matrix<T> &matrixB) const;

public:
  [[nodiscard]] std::unique_ptr<IMatrixMultiplication> clone() const override;

  Matrix<float> operator()(const Matrix<float> &matrixA, const Matrix<float> &matrixB) const override;

  Matrix<double> operator()(const Matrix<double> &matrixA, const Matrix<double> &matrixB) const override;

  Matrix<int> operator()(const Matrix<int> &matrixA, const Matrix<int> &matrixB) const override;
};


#endif // DMLIB_NAIVEMATRIXMULTIPLICATION_H
