#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAESTRO 0           // Id del proceso maestro (parking).
#define T_DORMIDO_MAX 10;   /* Usado para calcular un valor en el intervalo (1, T_DORMIDO_MAX), el cual
                            indicara cuanto tiempo permanecera el vehiculo dormido despues de ser aparcado */

#define OP_APARCAR 0        // Valor enviado al parking cuando se quiere aparcar
#define OP_SALIR 1          // Valor enviado al parking cuando se quiere salir de este

#define NUM_ARGUMENTOS 3    // Numero de atributos que se pasaran al parking en un mensaje
#define ARG_OPERACION 0     // Posicion que ocupa la operacion a realizar
#define ARG_TAMANO 1        // Posicion que ocupa el tamano del vehiculo
#define ARG_MATRICULA 2     // Posicion que ocupa el identificador del vehiculo

#define TAM_COCHE 1         // Tamano de un coche
#define TAM_CAMION 2        // Tamano de un camion

/// Definicion de los structs
typedef struct vehiculo_t vehiculo;

/// Desglose de los structs
typedef struct vehiculo_t {
    unsigned int matricula; // Matricula/identificador del vehiculo
    unsigned int tamano;    // Tamano del vehiculo
} vehiculo;