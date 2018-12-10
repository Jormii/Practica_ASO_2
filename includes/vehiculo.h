#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAESTRO 0         // Id del proceso maestro (parking).
#define T_DORMIDO_MAX 10; /* Usado para calcular un valor en el intervalo (1, T_DORMIDO_MAX), el cual \
                          indicara cuanto tiempo permanecera el vehiculo dormido despues de ser aparcado. */

#define OP_APARCAR 0 // Valor enviado al parking cuando se quiere aparcar.
#define OP_SALIR 1   // Valor enviado al parking cuando se quiere salir de este.

#define NUM_ARGUMENTOS 4       // Numero de atributos que se pasaran al parking en un mensaje.
#define ARG_OPERACION 0        // Posicion que ocupa la operacion a realizar.
#define ARG_TAMANO 1           // Posicion que ocupa el tamano del vehiculo.
#define ARG_MATRICULA_O_PISO 2 /* Posicion que ocupa el identificador del vehiculo o el piso en el que se encuentra \
                               aparcado. */
#define ARG_PLAZA 3            /* Posicion que ocupa la plaza que ocupa el vehiculo en el parking. Cuando se envia \
                               la operacion "OP_APARCAR", se ignora este valor. */

#define NUM_RETURNS 3          /* Numero de variables que se recibiran por parte del parking despues de enviar la operacion \
                               de aparque. */
#define RETURN_HABIA_PLAZA 0   // Indica si el parking encontro plaza para el vehiculo.
#define RETURN_PISO 1          // Piso en el que se aparco el vehiculo.
#define RETURN_PRIMERA_PLAZA 2 // Primera plaza que ocupa el vehiculo.

#define TAM_COCHE 1  // Tamano de un coche.
#define TAM_CAMION 2 // Tamano de un camion.

/// Declaracion de los structs
typedef struct vehiculo_t vehiculo; // Struct que contiene todas las variables necesarias para el control de un vehiculo.

/// Definicion de los structs
typedef struct vehiculo_t
{
    unsigned int matricula; // Matricula/identificador del vehiculo.
    unsigned int tamano;    // Tamano del vehiculo.
} vehiculo;