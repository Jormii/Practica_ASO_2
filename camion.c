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

    // Mandar señal de aparque
    MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);

    srand(time(NULL));
    int tiempo_dormir = rand() % T_DORMIDO_MAX + 1;
    sleep(tiempo_dormir);

    argumentos[ARG_OPERACION] = OP_SALIR;
    MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}
