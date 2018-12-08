#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "vehiculo.h"

#define PLAZA_NO_ENCONTRADA 0   // Valor devuelto por buscar_plaza() cuando no habia plazas para el coche.
#define PLAZA_ENCONTRADA 1      // Valor devuelto por buscar_plaza() cuando habia plaza para el coche.
#define PLAZA_VACIA 0           /* Las matriculas de los vehiculos son los ids de sus procesos. Dado que
                                el parking es el proceso 0, se utiliza este valor para plazas vacias. */

#define ARG_PISOS 1             // Indice en argv donde se almacena el numero de pisos del parking.
#define ARG_PLAZAS 2            // Indice en argv donde se almacena el numero de plazas por piso.

/// Definiciones de los structs
typedef struct parking_t parking;
typedef struct piso_t piso;

/**
 * Crea un parking de "pisos" pisos, cada uno con "plazas" plazas.
 */ 
void crear_parking(parking *park, int pisos, int plazas);

/**
 * Libera la memoria reclamada por crear_parking().
 */
void destruir_parking(parking *park);

/**
 * Itera los pisos del parking argumento en busca de un lugar donde aparcar un vehiculo con matricula
 * "matricula" y de tamano "tamano".
 * Si se encuentra sitio, se devuelve "PLAZA_ENCONTRADA" y se actualizan las variables del parking.
 * Si no se encuentra sitio, se devuelve "PLAZA_NO_ENCONTRADA".
 */
char buscar_plaza(unsigned int tamano, parking *park, unsigned int matricula);

/**
 * Itera los pisos del parking en busca del vehiculo con matricula "matricula", actualizando el parking
 * cuando se encuentre.
 */
void vaciar_plaza(unsigned int tamano, parking *park, unsigned int matricula);

/**
 * Funcion de debug que imprime el parking argumento.
 */
void imprimir_parking(const parking *park);

/// Desglose de los structs
typedef struct parking_t {
    int n_pisos;        // Numero de pisos del parking.
    int n_plazas;       // Numero de plazas por piso del parking.
    piso *pisos;        // Array con los pisos del parking.
} parking;

typedef struct piso_t {
    int plazas_libres;  // Numero de plazas no ocupadas en el piso.
    int *id_vehiculos;  // Array de tamano "n_plazas" que almacena las matriculas de los vehiculos.
} piso;