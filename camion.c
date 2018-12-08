#include "includes/vehiculo.h"

int main(int argc, char *argv[])
{
    // Se inicializa MPI
    MPI_Init(&argc, &argv);

    // Se crea el vehiculo
    vehiculo camion;
    MPI_Comm_rank(MPI_COMM_WORLD, &camion.matricula);
    camion.tamano = TAM_CAMION;
    unsigned int habia_plaza = 0;

    // Se configuran los argumentos que se pasaran al proceso parking
    unsigned int argumentos[NUM_ARGUMENTOS];

    // Se configura la variable de retorno
    unsigned int returns[NUM_RETURNS];

    while (1)
    {
        argumentos[ARG_OPERACION] = OP_APARCAR;
        argumentos[ARG_TAMANO] = camion.tamano;
        argumentos[ARG_MATRICULA_O_PISO] = camion.matricula;
        argumentos[ARG_PLAZA] = 0;

        // Mandar señal aparque
        do
        {
            MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);
            MPI_Recv(returns, NUM_RETURNS, MPI_UNSIGNED, MAESTRO, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            habia_plaza = returns[RETURN_HABIA_PLAZA];
            if (!habia_plaza)
            {
                sleep(1); // Dormir un segundo si no se encontro PLAZA en el que aparcar el coche
            }
        } while (!habia_plaza);

        // El proceso se duerme un tiempo aletorio entre (1, T_DORMIDO_MAX)
        srand(time(NULL));
        int tiempo_dormir = rand() % T_DORMIDO_MAX + 1;
        sleep(tiempo_dormir);

        // Tras despertarse, envia un mensaje al parking para salir de este
        argumentos[ARG_OPERACION] = OP_SALIR;
        argumentos[ARG_MATRICULA_O_PISO] = returns[RETURN_PISO];
        argumentos[ARG_PLAZA] = returns[RETURN_PRIMERA_PLAZA];

        MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);
        sleep(2);
    }

    // Se finaliza MPI
    MPI_Finalize();

    return 0;
}