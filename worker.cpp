#include <iostream>
#include <mpi.h>
#include <ostream>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm parent;
    MPI_Comm_get_parent(&parent);
    int rank, broadcastedValue;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // std::cout << "here" << std::endl;
    MPI_Bcast(&broadcastedValue, 1, MPI_INT, 0, parent);
    printf("Worker %d: Hello, we received %d \n", rank, broadcastedValue);
    // ... talk to parent over 'parent' intercommunicator ...
    MPI_Comm_disconnect(&parent);
    MPI_Finalize();
    return 0;
}
