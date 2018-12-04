#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAESTRO 0
#define T_DORMIDO_MAX 10;

#define OP_APARCAR 0
#define OP_SALIR 1

#define NUM_ARGUMENTOS 3
#define ARG_OPERACION 0
#define ARG_TAMANO 1
#define ARG_MATRICULA 2

#define TAM_COCHE 1
#define TAM_CAMION 2

typedef struct vehiculo_t vehiculo;

typedef struct vehiculo_t {
    unsigned int matricula;
    unsigned int tamano;
    // int piso_ocupado;
} vehiculo;