#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm inter;
  MPI_Comm_spawn(
      "./build/ParallelScalingWorker", MPI_ARGV_NULL, /*maxprocs*/ 4, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &inter,
      MPI_ERRCODES_IGNORE);
  printf("Manager: Hello");
  int passedInteger = 5;
  MPI_Bcast(&passedInteger, 1, MPI_INT, MPI_ROOT, inter);
  // ... exchange messages with children using 'inter' ...
  MPI_Comm_disconnect(&inter);
  MPI_Finalize();
  return 0;
}
