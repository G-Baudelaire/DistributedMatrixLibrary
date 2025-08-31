//
// Created by germain on 8/29/25.
//

#ifndef DMLIB_MPI_TYPE_TEMPLATES_H
#define DMLIB_MPI_TYPE_TEMPLATES_H

template<class T>
MPI_Datatype mpiType();

template<>
inline MPI_Datatype mpiType<int>() {
  return MPI_INT;
}

template<>
inline MPI_Datatype mpiType<float>() {
  return MPI_FLOAT;
}

template<>
inline MPI_Datatype mpiType<double>() {
  return MPI_DOUBLE;
}

template<>
inline MPI_Datatype mpiType<long>() {
  return MPI_LONG;
}

template<class T>
int dataType();

template<>
inline int dataType<int>() {
  return 0;
}

template<>
inline int dataType<float>() {
  return 1;
}

template<>
inline int dataType<double>() {
  return 2;
}

template<>
inline int dataType<long>() {
  return 3;
}
#endif // DMLIB_MPI_TYPE_TEMPLATES_H
