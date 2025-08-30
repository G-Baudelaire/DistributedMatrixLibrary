#include <iostream>
#include <mpi.h>
#include <mpi_type_templates.h>
#include <ostream>
#include <vector>

inline void scatterCount(int& elementCount, const MPI_Comm& parent) {
  MPI_Scatter(nullptr, 0, MPI_DATATYPE_NULL, &elementCount, 1, MPI_INT, 0, parent);
}

template<class T>
void broadcastScalar(T& scalar, const MPI_Comm& parent) {
  MPI_Bcast(&scalar, 1, mpiType<T>(), 0, parent);
}

template<class T>
void getDataFromParent(std::vector<T>& elements, const int elementCount, const MPI_Comm& parent) {
  MPI_Scatterv(nullptr, nullptr, nullptr, MPI_DATATYPE_NULL, elements.data(), elementCount, mpiType<T>(), 0, parent);
}

template<class T>
void returnDataToParent(const std::vector<T>& elements, const int elementCount, const MPI_Comm& parent) {
  MPI_Gatherv(elements.data(), elementCount, mpiType<T>(), nullptr, nullptr, nullptr, MPI_DATATYPE_NULL, 0, parent);
}

template<class T>
void run(MPI_Comm& parent) {
  int count;
  T scalar;

  broadcastScalar(scalar, parent);

  scatterCount(count, parent);
  std::vector<T> elements(count);

  getDataFromParent(elements, count, parent);

  for (T& element: elements) {
    element *= scalar;
  }
  returnDataToParent(elements, count, parent);
}

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
    default:
      returnCode = 1;
      break;
  }

  MPI_Comm_disconnect(&parent);
  MPI_Finalize();
  return returnCode;
}
