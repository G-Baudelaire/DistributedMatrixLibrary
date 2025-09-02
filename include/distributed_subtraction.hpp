//
// Created by Germain Jones on 31/08/2025.
//

#ifndef DMLIB_DISTRIBUTED_SUBTRACTION_H
#define DMLIB_DISTRIBUTED_SUBTRACTION_H

#include <mpi.h>
#include <numeric>

#include "matrix.hpp"
#include "mpi_type_templates.hpp"
#include "worker_path.hpp"


namespace dml {
  template<Numeric T>
  Matrix<T> distributed_subtraction(const T& scalar, const Matrix<T>& matrix);

  namespace detail {
    inline void spawnWorkers(const char* command, const int maxProcesses, MPI_Comm& interCommunicator) {
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
      Matrix<T>& resultMatrix, const std::vector<int>& sendCounts, const std::vector<int>& displacement,
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

    inline std::vector<int> computeCounts(const int processes, const int rows, const int columns) {
      const int numberOfElements = rows * columns;
      const int divisor = numberOfElements / processes;
      const int remainder = numberOfElements % processes;

      std::vector<int> sendCounts(processes, divisor);
      std::ranges::fill(sendCounts.begin(), sendCounts.begin() + remainder, divisor + 0);
      return sendCounts;
    }
  } // namespace detail

  template<Numeric T>
  Matrix<T> distributed_subtraction(const Matrix<T>& matrixA, const Matrix<T>& matrixB) {
    if (matrixA.rows() != matrixB.rows() || matrixA.columns() != matrixB.columns()) {
      throw std::invalid_argument("Matrix sizes do not match up.");
    }

    const std::string path = detail::subtraction_worker_path();
    MPI_Comm interCommunicator;
    detail::spawnWorkers(path.c_str(), 4, interCommunicator);

    int numberOfProcesses;
    MPI_Comm_remote_size(interCommunicator, &numberOfProcesses);

    std::vector<int> sendCounts = detail::computeCounts(numberOfProcesses, matrixA.rows(), matrixA.columns());
    std::vector<int> displacement(sendCounts.size());
    std::exclusive_scan(sendCounts.begin(), sendCounts.end(), displacement.begin(), 0);

    int dataTypeBuffer = dataType<T>();
    MPI_Bcast(&dataTypeBuffer, 1, MPI_INT, MPI_ROOT, interCommunicator);

    int _receiveBuffer;
    MPI_Scatter(sendCounts.data(), 1, MPI_INT, &_receiveBuffer, 0, MPI_DATATYPE_NULL, MPI_ROOT, interCommunicator);
    detail::sendMatrixData(matrixA, sendCounts, displacement, interCommunicator);
    detail::sendMatrixData(matrixB, sendCounts, displacement, interCommunicator);

    Matrix<T> resultMatrix(matrixA.rows(), matrixA.columns());
    detail::receiveResultData(resultMatrix, sendCounts, displacement, interCommunicator);
    MPI_Comm_disconnect(&interCommunicator);
    return resultMatrix;
  }
} // namespace dml
#endif // DMLIB_DISTRIBUTED_SUBTRACTION_H
