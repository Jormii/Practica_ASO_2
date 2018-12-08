#include "includes/vehiculo.h"

int main(int argc, char *argv[])
{
    // Se inicializa MPI
    MPI_Init(&argc, &argv);

    // Se crea el vehiculo
    vehiculo camion;
    MPI_Comm_rank(MPI_COMM_WORLD, &camion.matricula);
    camion.tamano = TAM_CAMION;

    // Se configuran los argumentos que se pasaran al proceso parking
    unsigned int argumentos[NUM_ARGUMENTOS];
    argumentos[ARG_OPERACION] = OP_APARCAR;
    argumentos[ARG_TAMANO] = camion.tamano;
    argumentos[ARG_MATRICULA] = camion.matricula;

    // Mandar señal de aparque
    MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);

    // El proceso se duerme un tiempo aletorio entre (1, T_DORMIDO_MAX)
    srand(time(NULL));
    int tiempo_dormir = rand() % T_DORMIDO_MAX + 1;
    sleep(tiempo_dormir);

    // Tras despertarse, envia un mensaje al parking para salir de este
    argumentos[ARG_OPERACION] = OP_SALIR;
    MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);

    // Se finaliza MPI
    MPI_Finalize();

    return 0;
}
