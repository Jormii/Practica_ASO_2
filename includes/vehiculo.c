#pragma once // ???
#include "vehiculo.h"

void vehiculo_bucle_principal(unsigned int tamano_vehiculo, int *argc, char **argv[])
{
    // Se inicializa MPI.
    MPI_Init(argc, argv);

    // Se crea el vehiculo.
    vehiculo vehi;
    MPI_Comm_rank(MPI_COMM_WORLD, &vehi.matricula);
    vehi.tamano = tamano_vehiculo;
    unsigned int habia_plaza = 0;

    // Se crea la variable de argumentos que se pasaran al proceso parking.
    unsigned int argumentos[NUM_ARGUMENTOS];

    // Se crea la variable de retorno.
    unsigned int returns[NUM_RETURNS];

    // El vehiculo envia mensajes de aparque y salida indefinidamente.
    while (1)
    {
        // Se configuran los argumentos para mandar el mensaje de aparque.
        argumentos[ARG_OPERACION] = OP_APARCAR;
        argumentos[ARG_TAMANO] = vehi.tamano;
        argumentos[ARG_MATRICULA_O_PISO] = vehi.matricula;
        argumentos[ARG_PLAZA] = 0;

        // Mandar senal de aparque. El vehiculo mandara el mensaje hasta que se encuentre plaza para este.
        do
        {
            // Se envia el mensaje.
            MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);

            // Se espera a la respuesta del parking para proceder.
            MPI_Recv(returns, NUM_RETURNS, MPI_UNSIGNED, MAESTRO, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            habia_plaza = returns[RETURN_HABIA_PLAZA];
            if (!habia_plaza)
            {
                sleep(1); // Dormir un segundo si no se encontro plaza en la que aparcar el vehiculo.
            }
        } while (!habia_plaza);

        // El proceso se duerme un tiempo aletorio en el intervalo (1, T_DORMIDO_MAX).
        srand(time(NULL)); // ¿GENERA UNA SEMILLA DISTINTA CADA VEZ QUE ES INVOCADO?
        int tiempo_dormir = rand() % T_DORMIDO_MAX + 1;
        sleep(tiempo_dormir);

        // Tras despertarse, envia un mensaje al parking para salir de este. El argumento "tamano" es constante.
        argumentos[ARG_OPERACION] = OP_SALIR;
        argumentos[ARG_MATRICULA_O_PISO] = returns[RETURN_PISO];
        argumentos[ARG_PLAZA] = returns[RETURN_PRIMERA_PLAZA];

        MPI_Send(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MAESTRO, 0, MPI_COMM_WORLD);
        sleep(2); /* Duerme dos segundos antes de volver a mandar otro mensaje de aparque para permitir a los vehiculos
                    que se han dormido en el cuerpo del bucle mandar la senal de aparque antes que este. */
    }

    // Se finaliza MPI.
    MPI_Finalize();
}