//
// Created by Germain Jones on 31/08/2025.
//

#include <iostream>
#include <mpi.h>
#include <mpi_type_templates.hpp>

namespace {
  void receive_count(int& elementCount, const MPI_Comm& parent) {
    MPI_Scatter(nullptr, 0, MPI_DATATYPE_NULL, &elementCount, 1, MPI_INT, 0, parent);
  }

  template<class T>
  void receive_matrix_data(T elements[], const int receiveCount, const MPI_Comm& parent) {
    MPI_Scatterv(nullptr, nullptr, nullptr, MPI_DATATYPE_NULL, elements, receiveCount, mpiType<T>(), 0, parent);
  }

  template<class T>
  void send_result_data(const T elements[], const int elementCount, const MPI_Comm& parent) {
    MPI_Gatherv(elements, elementCount, mpiType<T>(), nullptr, nullptr, nullptr, MPI_DATATYPE_NULL, 0, parent);
  }

  template<class T>
  void run(MPI_Comm& parent) {
    int count;

    receive_count(count, parent);

    T matrixAData[count];
    T matrixBData[count];
    receive_matrix_data(matrixAData, count, parent);
    receive_matrix_data(matrixBData, count, parent);

    for (int i = 0; i < count; i++) {
      matrixAData[i] -= matrixBData[i];
    }

    send_result_data(matrixAData, count, parent);
  }
} // namespace

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm parent;
  MPI_Comm_get_parent(&parent);

  int dtype = 0; // 0=int, 1=float, 2=double
  MPI_Bcast(&dtype, 1, MPI_INT, 0, parent);

  int returnCode = 0;
  switch (dtype) {
    case 0:
      run<int>(parent);
      break;
    case 1:
      run<float>(parent);
      break;
    case 2:
      run<double>(parent);
      break;
    case 3:
      run<long>(parent);
      break;
    default:
      returnCode = 1;
      break;
  }

  MPI_Comm_disconnect(&parent);
  MPI_Finalize();
  return returnCode;
}
