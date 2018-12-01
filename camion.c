#include "includes/vehiculo.h"

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    vehiculo camion;
    MPI_Comm_rank(MPI_COMM_WORLD, &camion.matricula);
    camion.tamano = TAM_CAMION;

    unsigned int argumentos[NUM_ARGUMENTOS];
    argumentos[ARG_OPERACION] = OP_APARCAR;
    argumentos[ARG_TAMANO] = camion.tamano;
    argumentos[ARG_MATRICULA] = camion.matricula;

    // printf("Camion con matricula: %d\n", camion.matricula);

    // Mandar señal de aparque
    MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}
