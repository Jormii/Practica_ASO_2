#include "./includes/parking.h"

void crear_parking(parking *park, int pisos, int plazas)
{
    // Inicializacion del struct parking
    park->n_pisos = pisos;
    park->n_plazas = plazas;
    park->pisos = (piso*)calloc(pisos, sizeof(piso));

    // Inicializacion de cada uno de los structs piso dentro del parking
    for (int p = 0; p < pisos; p++)
    {
        piso *pi = &(park->pisos[p]);
        pi->id_vehiculos = (int*)calloc(plazas, sizeof(int));
        pi->plazas_libres = plazas;

        for (int pz = 0; pz != plazas; ++pz)
        {
            pi->id_vehiculos[pz] = PLAZA_VACIA;
        }
    }
}

void destruir_parking(parking *park)
{   
    for (int p = 0; p < park->n_pisos; ++p)
    {
        free(park->pisos[p].id_vehiculos);  // ¿Memory leak?
    }
    free(park->pisos);
}

char buscar_plaza(unsigned int tamano, parking *park, unsigned int matricula)
{
    // Se iteran todos los pisos del parking
    for (int p = 0; p < park->n_pisos; p++)
    {
        int *p_pz_libres = &(park->pisos[p].plazas_libres);

        printf("{P%d}: %d plazas libres de %d\n", p, *p_pz_libres, park->n_plazas);

        // Si hay suficientes plazas libres como para alojar el vehiculo, buscar en el piso
        if (*p_pz_libres >= tamano)
        {
            int cnt = 0;
            int *plazas = park->pisos[p].id_vehiculos;

            // Iterar las plazas del piso en busca de plazas vacias consecutivas donde aparcar el vehiculo
            for (int pz = 0; pz < park->n_plazas; pz++)
            {
                cnt = (plazas[pz] == PLAZA_VACIA) ? cnt + 1 : 0;

                // Si se han encontrado las plazas, aparcarlo
                if (cnt == tamano)
                {
                    *p_pz_libres -= tamano;

                    int primera_plaza = pz - tamano + 1;
                    for (int i = primera_plaza; i <= pz; i++)
                    {
                        plazas[i] = matricula;
                    }
                    return PLAZA_ENCONTRADA;    // Devolver TRUE
                }
            }
        }
    }
    return PLAZA_NO_ENCONTRADA;     // No se ha encontrado. Devolver FALSE
}

void vaciar_plaza(unsigned int tamano, parking *park, unsigned int matricula)
{
    // Se iteran todos los pisos del parking
    for (int p = 0; p < park->n_pisos; p++)
    {
        // Se iteran las plazas de todos los pisos hasta que se encuentre la matricula argumento
        int *plazas = park->pisos[p].id_vehiculos;
        for (int pz = 0; pz < park->n_plazas; pz++)
        {
            // Una vez se encuentre, desalojar el coche
            if (plazas[pz] == matricula)
            {
                int primera_plaza = pz - tamano + 1;
                for (int i = primera_plaza; i <= pz; i++)
                {
                    plazas[i] = PLAZA_VACIA;
                }
            }
        }
    }
}

void imprimir_parking(const parking *park)
{
    for (int p = 0; p != park->n_pisos; ++p)
    {
        piso *pi = &(park->pisos[p]);
        printf("{Planta %d}\t", p);
        for (int pz = 0; pz != park->n_plazas; ++pz)
        {
            printf("[%d] ", pi->id_vehiculos[pz]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    // Se inicializa MPI
    MPI_Init(&argc, &argv);

    // Se crea el parking
    int pisos = atoi(argv[ARG_PISOS]);
    int plazas = atoi(argv[ARG_PLAZAS]);
    parking park;
    crear_parking(&park, pisos, plazas);

    // Configuracion de las variables de control del bucle principal
    unsigned int argumentos[NUM_ARGUMENTOS];
    int n_esclavos = 0;     // Numero de procesos esclavos aun activos
    MPI_Comm_size(MPI_COMM_WORLD, &n_esclavos);
    n_esclavos -= 1;        // Se resta el proceso parking
    printf("Numero de coches y camiones: %d\n\n", n_esclavos);

    while (n_esclavos > 0)
    {
        // Recibir un mensaje enviado por alguno de los vehiculos
        MPI_Recv(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Parsear el mensaje
        unsigned int operacion = argumentos[ARG_OPERACION];
        unsigned int tamano = argumentos[ARG_TAMANO];
        unsigned int matricula = argumentos[ARG_MATRICULA];

        printf("El parking ha recibido un mensaje. Argumentos:\n");
        printf("\tOperacion: %s\n", (operacion == OP_APARCAR) ? "Aparcar" : "Salir");
        printf("\tTamano del vehiculo: %u\n", tamano);
        printf("\tMatricula del vehiculo: %u\n", matricula);

        // Actuar en funcion del codigo de operacion enviado por el vehiculo
        switch (operacion)
        {
        case OP_APARCAR:
            buscar_plaza(tamano, &park, matricula);
            break;
        case OP_SALIR:
            vaciar_plaza(tamano, &park, matricula);
            n_esclavos--;
            printf("\n\nQuedan %d coches en el parking/por aparcar\n\n", n_esclavos);
            break;
        default:
            fprintf(stderr, "Error: El codigo de operacion {%d} es extrano\n", operacion);
        }
        imprimir_parking(&park);
        printf("\n");
    }

    // Se finaliza MPI y se destruye el parking
    MPI_Finalize();
    destruir_parking(&park);

    return 0;
}
