#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "vehiculo.h"
#include "queue.c"

#define PLAZA_NO_ENCONTRADA 0 // Valor devuelto por buscar_plaza() cuando no hay espacio para el vehiculo.
#define PLAZA_ENCONTRADA 1    // Valor devuelto por buscar_plaza() cuando hay espacio para el vehiculo.
#define PLAZA_VACIA 0         /* Las matriculas de los vehiculos son los ids de sus procesos. Dado que el parking es \
                              el proceso 0, se utiliza este valor para indicar las plazas vacias. */

#define ARGC_NECESARIO 3 /* Argc debe valer 3 para poder establecer los valores de "n_pisos" y "n_plazas" a los \
                         indicados en los argumentos del programa. */
#define ARG_PISOS 1      // Indice en argv donde se almacena el numero de pisos del parking.
#define ARG_PLAZAS 2     // Indice en argv donde se almacena el numero de plazas por piso.

#define MIN_PISOS 1  // Numero de pisos minimo que debe tener el parking.
#define MIN_PLAZAS 2 // Numero de plazas por piso minimo que debe tener el parking.

/// Declaracion de los structs
typedef struct parking_t parking; // Contiene las variables necesarias para manejar el parking.
typedef struct piso_t piso;       // Contiene las variables necesarias para administrar un piso del parking.

/**
 * Crea un parking de "pisos" pisos, cada uno con "plazas" plazas.
 */
void crear_parking(parking *park, int pisos, int plazas);

/**
 * Libera la memoria reclamada por crear_parking().
 */
void destruir_parking(parking *park);

/**
 * Itera los pisos del parking argumento en busca de un lugar donde aparcar un vehiculo con matricula "matricula"
 * y de tamano "tamano".
 * Si se encuentra plaza, se devuelve "PLAZA_ENCONTRADA" y se actualizan las variables del parking.
 * Si no se encuentra plaza, se devuelve "PLAZA_NO_ENCONTRADA".
 * La variable "returns_array" contendra los valores de retorno que se enviaran al vehiculo que mando la operacion de
 * aparque en caso de que el valor devuelto sea "PLAZA_ENCONTRADA".
 */
char buscar_plaza(parking *park, unsigned int *returns_array, unsigned int matricula, unsigned int tamano);

/**
 * Libera las plazas del parking ocupadas por el vehiculo de tamano "tamano" aparcado en el piso "piso", siendo
 * "primera_plaza" la primera plaza que ocupa en dicho piso.
 */
void vaciar_plaza(parking *park, unsigned int tamano, unsigned int piso, unsigned int primera_plaza);

/**
 * Introuduce el vehiculo en una cola
 */
void queue_vehiculo(Queue *cola, unsigned int tamano, unsigned int matricula);

/**
 * Funcion que imprime una representacion en texto del parking.
 */
void imprimir_parking(const parking *park);

/**
 * Funcion que imprime la cola de vehiculos esperando por una plaza.
 */
void imprimir_cola(const Queue *cola);

/// Implementacion de los structs
typedef struct parking_t
{
    unsigned int n_pisos;  // Numero de pisos del parking.
    unsigned int n_plazas; // Numero de plazas por piso del parking.
    piso *pisos;  // Array con los pisos del parking.
} parking;

typedef struct piso_t
{
    unsigned int plazas_libres; // Numero de plazas no ocupadas en el piso.
    unsigned int *id_vehiculos; // Array de tamano "n_plazas" que almacena las matriculas de los vehiculos aparcados en la planta.
} piso;