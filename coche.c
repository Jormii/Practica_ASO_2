#include "includes/vehiculo.h"

int main(int argc, char *argv[])
{
    // Se inicializa MPI
    MPI_Init(&argc, &argv);

    // Se crea el vehiculo
    vehiculo coche;
    MPI_Comm_rank(MPI_COMM_WORLD, &coche.matricula);
    coche.tamano = TAM_COCHE;

    // Se configuran los argumentos que se pasaran al proceso parking
    unsigned int argumentos[NUM_ARGUMENTOS];
    argumentos[ARG_OPERACION] = OP_APARCAR;
    argumentos[ARG_TAMANO] = coche.tamano;
    argumentos[ARG_MATRICULA] = coche.matricula;

    // Mandar señal de aparque
    MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);

    // El proceso se duerme un tiempo aleatorio entre (1, T_DORMIDO_MAX)
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
