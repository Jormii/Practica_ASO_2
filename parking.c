#include "./includes/parking.h"

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

char buscar_plaza(unsigned int tamano, parking *park, unsigned int matricula)
{
    printf("Tamano del vehiculo a aparcar: %d\n", tamano);

    for (int p = 0; p < PISOS; ++p)
    {
        int *p_pz_libres = &(park->pisos[p]->plazas_libres);

        printf("{P%d}: %d plazas libres de %d\n", p, *p_pz_libres, PLAZAS);

        if (*p_pz_libres >= tamano)
        {
            int cnt = 0;
            int *plazas = park->pisos[p]->id_vehiculos;
            for (int pz = 0; pz < PLAZAS; ++pz)
            {
                cnt = (plazas[pz] == PLAZA_VACIA) ? cnt + 1 : 0;

                if (cnt == tamano)
                {
                    *p_pz_libres = *p_pz_libres - tamano;

                    int primera_plaza = pz - tamano + 1;
                    for (int i = primera_plaza; i <= pz; i++)
                    {
                        plazas[i] = matricula;
                    }
                    return PLAZA_ENCONTRADA;
                }
            }
        }
    }
    return PLAZA_NO_ENCONTRADA;
}

void imprimir_parking(const parking *park)
{
    for (int p = 0; p != PISOS; ++p)
    {
        piso *pi = park->pisos[p];
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
    MPI_Init(&argc, &argv);

    parking park;
    crear_parking(&park);

    unsigned int argumentos[NUM_ARGUMENTOS];
    while (1)
    {
        for (int i = 0; i != NUM_ARGUMENTOS; ++i)
        {
            MPI_Recv(&(argumentos[i]), 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        unsigned int operacion = argumentos[ARG_OPERACION];
        unsigned int tamano = argumentos[ARG_TAMANO];
        unsigned int matricula = argumentos[ARG_MATRICULA];

        switch (operacion)
        {
            case OP_APARCAR:
                buscar_plaza(tamano, &park, matricula);
                break;
            case OP_SALIR:
                break;
            default:
                fprintf(stderr, "Error: El codigo de operacion {%d} es extrano\n", operacion);
        }
        imprimir_parking(&park);
    }

    MPI_Finalize();
    destruir_parking(&park);

    return 0;
}