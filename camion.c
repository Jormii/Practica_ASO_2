#include "vehiculo.h"

int main(int argc, char *argv[]) {
    int n_proc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &n_proc);

    printf("Camion con id: %d\n", n_proc);

    return 0;
}