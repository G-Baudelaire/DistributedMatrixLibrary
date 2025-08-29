//
// Created by germain on 8/29/25.
//

#ifndef DMLIB_DISTRIBUTEDMULTIPLICATION_H
#define DMLIB_DISTRIBUTEDMULTIPLICATION_H
#include <IMultiplication.h>


class DistributedMultiplication final : public IMultiplication {
public:
  [[nodiscard]] std::unique_ptr<IMultiplication> clone() const override;
  Matrix<float> operator()(const float &scalar, const Matrix<float> &matrix) const override;
  Matrix<double> operator()(const double &scalar, const Matrix<double> &matrix) const override;
  Matrix<int> operator()(const int &scalar, const Matrix<int> &matrix) const override;
};


#endif // DMLIB_DISTRIBUTEDMULTIPLICATION_H
