#pragma once
#include "vehiculo.h"

void vehiculo_bucle_principal(unsigned int tamano_vehiculo, int *argc, char **argv[])
{
    // Se inicializa MPI y una semilla.
    MPI_Init(argc, argv);
    srand(time(NULL));

    // Se crea el vehiculo.
    unsigned int tamano = tamano_vehiculo;
    unsigned int matricula;
    MPI_Comm_rank(MPI_COMM_WORLD, &matricula);

    // Se crea la variable de argumentos que se pasaran al proceso parking.
    unsigned int argumentos[NUM_ARGUMENTOS];

    // Se crea la variable de retorno.
    unsigned int returns[NUM_RETURNS];

    // El vehiculo envia mensajes de aparque y salida indefinidamente.
    while (1)
    {
        // Se configuran los argumentos para mandar el mensaje de aparque.
        argumentos[ARG_OPERACION] = OP_APARCAR;
        argumentos[ARG_TAMANO] = tamano;
        argumentos[ARG_MATRICULA_O_PISO] = matricula;
        argumentos[ARG_PLAZA] = 0;

        // Se envia la senal de aparque.
        MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);

        // Se espera a que el parking confirme que hay espacio para este vehiculo.
        MPI_Recv(returns, NUM_RETURNS, MPI_UNSIGNED, MAESTRO, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // El proceso se duerme un tiempo aletorio en el intervalo (1, T_DORMIDO_MAX).
        int tiempo_dormir = rand() % T_DORMIDO_MAX + 1;
        sleep(tiempo_dormir);

        // Tras despertarse, envia un mensaje al parking para salir de este. El argumento "tamano" es constante.
        argumentos[ARG_OPERACION] = OP_SALIR;
        argumentos[ARG_MATRICULA_O_PISO] = returns[RETURN_PISO];
        argumentos[ARG_PLAZA] = returns[RETURN_PRIMERA_PLAZA];

        MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);

        // Duerme un tiempo aleatorio antes de volver a mandar otro mensaje de aparque.
        tiempo_dormir = rand() % T_DORMIDO_MAX + 1;
        sleep(tiempo_dormir);
    }

    // Se finaliza MPI.
    MPI_Finalize();
}