#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "vehiculo.h"

#define MIN_PISOS 1
#define MIN_PLAZAS 2

#define PISOS 2
#define PLAZAS 6

typedef struct parking_t parking;
typedef struct piso_t piso;

void crear_parking(parking *park, int pisos, int plazas_por_piso);
void destruir_parking(parking *park);
piso *buscar_plaza(vehiculo vehi);
void imprimir_parking(const parking *park);

typedef struct parking_t {
    int n_pisos;
    int n_plazas;
    piso *pisos;
} parking;

typedef struct piso_t {
    int n_planta;
    int *id_vehiculos;
} piso;