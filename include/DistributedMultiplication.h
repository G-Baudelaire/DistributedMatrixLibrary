//
// Created by germain on 8/29/25.
//

#ifndef DMLIB_DISTRIBUTEDMULTIPLICATION_H
#define DMLIB_DISTRIBUTEDMULTIPLICATION_H
#include <IMultiplication.h>

#include <utility>


class DistributedMultiplication final : public IMultiplication {
  std::string workerExecutableName;
  const int maxProcesses;

  template<Numeric T>
  Matrix<T> genericMultiplication(const T &scalar, const Matrix<T> &matrix) const;

  static std::vector<int> computeCounts(int processes, int rows, int columns);

public:
  explicit DistributedMultiplication(std::string workerFilePath_, const int maxProcesses_) :
      workerExecutableName(std::move(workerFilePath_)), maxProcesses(maxProcesses_) {}

  [[nodiscard]] std::unique_ptr<IMultiplication> clone() const override;

  Matrix<float> operator()(const float &scalar, const Matrix<float> &matrix) const override;

  Matrix<double> operator()(const double &scalar, const Matrix<double> &matrix) const override;

  Matrix<int> operator()(const int &scalar, const Matrix<int> &matrix) const override;
};


#endif // DMLIB_DISTRIBUTEDMULTIPLICATION_H
