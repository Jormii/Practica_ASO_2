#include "./includes/parking.h"
#include <time.h>

void crear_parking(parking *park)
{
    for (int p = 0; p < PISOS; ++p)
    {
        piso **pi = &(park->pisos[p]);
        *pi = (piso *)malloc(sizeof(piso));
        (*pi)->n_piso = p;
        (*pi)->plazas_libres = PLAZAS;

        for (int pz = 0; pz != PLAZAS; ++pz)
        {
            (*pi)->id_vehiculos[pz] = PLAZA_VACIA;
        }
    }
}

void destruir_parking(parking *park)
{
    for (int p = 0; p < PISOS; ++p)
    {
        free(park->pisos[p]);
    }
}

char buscar_plaza(vehiculo *vehi, parking *park)
{
    printf("Tamano del vehiculo a aparcar: %d\n", vehi->tamano);

    for (int p = 0; p < PISOS; ++p)
    {
        int p_pz_libres = park->pisos[p]->plazas_libres;

        printf("{P%d}: %d plazas libres de %d\n", p, p_pz_libres, PLAZAS);

        if (p_pz_libres >= vehi->tamano)
        {
            int cnt = 0;
            int *plazas = park->pisos[p]->id_vehiculos;
            for (int pz = 0; pz < PLAZAS; ++pz)
            {
                cnt = (plazas[pz] == PLAZA_VACIA) ? cnt + 1 : 0;

                if (cnt == vehi->tamano)
                {
                    park->pisos[p]->plazas_libres -= vehi->tamano;

                    vehi->piso_ocupado = p;
                    vehi->plaza_ocupada = pz - vehi->tamano + 1;

                    // Error aqui
                    for (int i = vehi->plaza_ocupada; i != vehi->tamano; ++i)
                    {
                        plazas[i] = vehi->matricula;
                    }
                    return PLAZA_ENCONTRADA;
                }
            }
        }
    }

    return PLAZA_NO_ENCONTRADA;
}

void imprimir_parking(parking park)
{
    for (int p = 0; p != PISOS; ++p)
    {
        piso *pi = park.pisos[p];
        printf("{Planta %d}\t", p);
        for (int pz = 0; pz != PLAZAS; ++pz)
        {
            printf("[%d] ", pi->id_vehiculos[pz]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    parking park;
    crear_parking(&park);

    MPI_Init(&argc, &argv);

    // imprimir_parking(park);

    vehiculo vehi;
    vehi.tamano = 1;
    vehi.matricula = 1;

    char hay_sitio = buscar_plaza(&vehi, &park);
    printf("El vehiculo tiene sitio: %d\n", hay_sitio);
    if (hay_sitio)
    {
        printf("Ocupada/s la/s plaza/s %d a %d en el piso %d\n", vehi.plaza_ocupada,
               vehi.plaza_ocupada + vehi.tamano - 1, vehi.piso_ocupado);
    }

    imprimir_parking(park);

    MPI_Finalize();
    destruir_parking(&park);

    return 0;
}