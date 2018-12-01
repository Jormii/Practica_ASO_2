#include "mpi.h"
#include <stdio.h>

#define NUM_ARGUMENTOS 3
#define ARG_OPERACION 0
#define ARG_TAMANO 1
#define ARG_MATRICULA 2

#define TAM_COCHE 1
#define TAM_CAMION 2

#define OP_APARCAR 1
#define OP_SALIR 2

typedef struct vehiculo_t vehiculo;

typedef struct vehiculo_t {
    unsigned int matricula;
    unsigned int tamano;
    // int piso_ocupado;
} vehiculo;