#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "vehiculo.h"

#define PLAZA_NO_ENCONTRADA 0 // Valor devuelto por buscar_plaza() cuando no habia plazas para el coche.
#define PLAZA_ENCONTRADA 1    // Valor devuelto por buscar_plaza() cuando habia plaza para el coche.
#define PLAZA_VACIA 0         /* Las matriculas de los vehiculos son los ids de sus procesos. Dado que el parking es \
                              el proceso 0, se utiliza este valor para indicar las plazas vacias. */

#define ARGC_NECESARIO 3 /* Argc debe valer 3 para poder establecer los valores de "n_pisos" y n_argumentos a los \
                         argumentos del programa. */
#define ARG_PISOS 1      // Indice en argv donde se almacena el numero de pisos del parking.
#define ARG_PLAZAS 2     // Indice en argv donde se almacena el numero de plazas por piso.

#define MIN_PISOS 1  // Numero de pisos minimo que debe tener el parking.
#define MIN_PLAZAS 2 // Numero de plazas por piso minimo que debe tener el parking.

/// Declaraciones de los structs
typedef struct parking_t parking; // Contiene las variables necesarias para manejar el parking.
typedef struct piso_t piso;       // Contiene las variables necesarias para controlar un piso.
typedef struct returns_t returns; /* Struct que contiene las variables que seran devueltas al vehiculo despues
                                    de invocar buscar_plaza(). */

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
 * La variable "ret" contendra los valores de retorno que se enviaran al vehiculo que mando el mensaje al parking.
 */
void buscar_plaza(parking *park, returns *ret, unsigned int matricula, unsigned int tamano);

/**
 * Libera las plazas del parking ocupadas por el vehiculo de tamano "tamano" aparcado en el piso "piso", siendo
 * "primera_plaza" la primera plaza que ocupa en el piso.
 */
void vaciar_plaza(parking *park, unsigned int tamano, unsigned int piso, unsigned int primera_plaza);

/**
 * Funcion que imprime el estado del parking argumento.
 */
void imprimir_parking(const parking *park);

/// Definiciones de los structs
typedef struct parking_t
{
    int n_pisos;  // Numero de pisos del parking.
    int n_plazas; // Numero de plazas por piso del parking.
    piso *pisos;  // Array con los pisos del parking.
} parking;

typedef struct piso_t
{
    int plazas_libres; // Numero de plazas no ocupadas en el piso.
    int *id_vehiculos; // Array de tamano "n_plazas" que almacena las matriculas de los vehiculos.
} piso;

typedef struct returns_t
{
    unsigned int habia_plaza;   // Indica si se encontro plaza para el vehiculo en el parking.
    unsigned int piso;          // Piso en el que se encontro plaza para el vehiculo.
    unsigned int primera_plaza; // Primera plaza que ocupa el vehiculo en el piso "piso".
} returns;