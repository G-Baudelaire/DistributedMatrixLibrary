//
// Created by germain on 8/29/25.
//

#include "../include/DistributedMultiplication.h"

#include <iostream>
#include <mpi.h>
#include <numeric>
#include "../include/mpi_type_templates.h"

namespace {
  void spawnWorkers(const char* command, const int maxProcesses, MPI_Comm& interCommunicator) {
    MPI_Comm_spawn(
      command,
      MPI_ARGV_NULL,
      maxProcesses,
      MPI_INFO_NULL,
      0,
      MPI_COMM_WORLD,
      &interCommunicator,
      MPI_ERRCODES_IGNORE
    );
  }

  template<class T>
  void receiveResultData(
    Matrix<T> &resultMatrix, const std::vector<int>& sendCounts, const std::vector<int>& displacement,
    const MPI_Comm& interCommunicator
  ) {
    MPI_Gatherv(
      nullptr,
      0,
      MPI_DATATYPE_NULL,
      resultMatrix.data(),
      sendCounts.data(),
      displacement.data(),
      mpiType<T>(),
      MPI_ROOT,
      interCommunicator
    );
  }

  template<class T>
  void sendMatrixData(
    Matrix<T> matrix, const std::vector<int>& sendCounts, const std::vector<int>& displacement,
    const MPI_Comm& interCommunicator
  ) {
    std::vector<int> receiveBuffer;
    MPI_Scatterv(
      matrix.data(),
      sendCounts.data(),
      displacement.data(),
      mpiType<T>(),
      receiveBuffer.data(),
      0,
      MPI_DATATYPE_NULL,
      MPI_ROOT,
      interCommunicator
    );
  }
} // namespace

template<Numeric T>
Matrix<T> DistributedMultiplication::genericMultiplication(const T& scalar, const Matrix<T>& matrix) const {
  MPI_Comm interCommunicator;
  spawnWorkers(workerExecutableName.c_str(), maxProcesses, interCommunicator);

  int numberOfProcesses;
  MPI_Comm_remote_size(interCommunicator, &numberOfProcesses);

  std::vector<int> sendCounts = computeCounts(numberOfProcesses, matrix.rows(), matrix.columns());
  std::vector<int> displacement(sendCounts.size());
  std::exclusive_scan(sendCounts.begin(), sendCounts.end(), displacement.begin(), 0);

  int dataTypeBuffer = dataType<T>();
  MPI_Bcast(&dataTypeBuffer, 1, MPI_INT, MPI_ROOT, interCommunicator);

  T scalarBuffer = scalar;
  MPI_Bcast(&scalarBuffer, 1, mpiType<T>(), MPI_ROOT, interCommunicator);

  int _receiveBuffer;
  MPI_Scatter(sendCounts.data(), 1, mpiType<T>(), &_receiveBuffer, 0, MPI_DATATYPE_NULL, MPI_ROOT, interCommunicator);
  sendMatrixData(matrix, sendCounts, displacement, interCommunicator);

  Matrix<T> resultMatrix(matrix.rows(), matrix.columns(), 0);
  receiveResultData(resultMatrix, sendCounts, displacement, interCommunicator);
  MPI_Comm_disconnect(&interCommunicator);
  return resultMatrix;
}

std::unique_ptr<IMultiplication> DistributedMultiplication::clone() const {
  return std::make_unique<DistributedMultiplication>(*this);
}

std::vector<int> DistributedMultiplication::computeCounts(const int processes, const int rows, const int columns) {
  const int numberOfElements = rows * columns;
  const int divisor = numberOfElements / processes;
  const int remainder = numberOfElements % processes;

  std::vector<int> sendCounts(processes, divisor);
  std::ranges::fill(sendCounts.begin(), sendCounts.begin() + remainder, divisor + 1);
  return sendCounts;
}

Matrix<float> DistributedMultiplication::operator()(const float& scalar, const Matrix<float>& matrix) const {
  return genericMultiplication(scalar, matrix);
}

Matrix<double> DistributedMultiplication::operator()(const double& scalar, const Matrix<double>& matrix) const {
  return genericMultiplication(scalar, matrix);
}

Matrix<int> DistributedMultiplication::operator()(const int& scalar, const Matrix<int>& matrix) const {
  return genericMultiplication(scalar, matrix);
}
