#include "mpi.h"
#include <stdio.h>

#define MIN_TAMANO 1
#define OP_APARCAR 1
#define OP_SALIR 2

typedef struct vehiculo_t vehiculo;

typedef struct vehiculo_t {
    int matricula;
    int tamano;
    int piso_ocupado;
    int plaza_ocupada;
} vehiculo;