#include "./includes/parking.h"

void crear_parking(parking *park, int pisos, int plazas) {
    park = (parking*)malloc(sizeof(parking));
    
    park->n_pisos = pisos;
    park->n_plazas = plazas;
    park->pisos = (piso*)calloc(pisos, sizeof(piso));

    // Error a partir de aqui

    piso **park_pisos = &(park->pisos);
    for (int i = 0; i != pisos; ++i) {
        park_pisos[i]->n_planta = i;
        park_pisos[i]->id_vehiculos = (int*)calloc(plazas, sizeof(int));
    }
}

void destruir_parking(parking *park) {
    for (int p = 0; p != park->n_pisos; ++p) {
        piso *pi = &(park->pisos[p]);
        free(pi->id_vehiculos);
        free(pi);
    }
    free(park);
}

void imprimir_parking(const parking *park) {
    for (int p = 0; p != park->n_pisos; ++p) {
        piso *pi = &(park->pisos[p]);
        printf("{Planta %d}\t", p);
        for (int pz = 0; pz != park->n_plazas; ++pz) {
            printf("[%d] ", pi->id_vehiculos[pz]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    parking *park;
    crear_parking(park, PISOS, PLAZAS);

    // MPI_Init(&argc, &argv);

    imprimir_parking(park);

    // MPI_Finalize();
    destruir_parking(park);

    return 0;
}