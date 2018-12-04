#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "vehiculo.h"

#define PLAZA_NO_ENCONTRADA 0
#define PLAZA_ENCONTRADA 1
#define PLAZA_VACIA 0

#define ARG_PISOS 1
#define ARG_PLAZAS 2

typedef struct parking_t parking;
typedef struct piso_t piso;

void crear_parking(parking *park, int pisos, int plazas);
void destruir_parking(parking *park);
char buscar_plaza(unsigned int tamano, parking *park, unsigned int matricula);
void vaciar_plaza(unsigned int tamano, parking *park, unsigned int matricula);
void imprimir_parking(const parking *park);

typedef struct parking_t {
    int n_pisos;
    int n_plazas;
    piso *pisos;
} parking;

typedef struct piso_t {
    int plazas_libres;
    int *id_vehiculos;
} piso;