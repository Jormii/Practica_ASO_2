#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "vehiculo.h"

#define PLAZA_NO_ENCONTRADA 0
#define PLAZA_ENCONTRADA 1
#define PLAZA_VACIA 0

#define PISOS 3
#define PLAZAS 5

typedef struct parking_t parking;
typedef struct piso_t piso;

void crear_parking(parking *park);
void destruir_parking(parking *park);
char buscar_plaza(vehiculo *vehi, parking *park);
void imprimir_parking(parking park);

typedef struct parking_t {
    piso *pisos[PISOS];
} parking;

typedef struct piso_t {
    int n_piso;
    volatile int plazas_libres;
    int id_vehiculos[PLAZAS];
} piso;