#include "./includes/parking.h"

void crear_parking(parking *park, int pisos, int plazas)
{
    // Inicializacion del struct parking.
    park->n_pisos = (unsigned int)pisos;
    park->n_plazas = (unsigned int)plazas;
    park->pisos = (piso *)calloc(pisos, sizeof(piso));

    // Inicializacion de cada uno de los structs piso dentro del parking.
    for (int p = 0; p < pisos; p++)
    {
        piso *pi = &(park->pisos[p]);
        pi->id_vehiculos = (int *)calloc(plazas, sizeof(int));
        pi->plazas_libres = (unsigned int)plazas;

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
        free(park->pisos[p].id_vehiculos);
    }
    free(park->pisos);
}

char buscar_plaza(parking *park, unsigned int *returns_array, unsigned int matricula, unsigned int tamano)
{
    // Se iteran todos los pisos del parking.
    for (int p = 0; p < park->n_pisos; p++)
    {
        int *p_pz_libres = &(park->pisos[p].plazas_libres);

        printf("{P%d}: %d plazas libres de %d\n", p, *p_pz_libres, park->n_plazas);

        // Si hay suficientes plazas libres como para alojar el vehiculo, buscar en el piso.
        if (*p_pz_libres >= tamano)
        {
            int cnt = 0;
            int *plazas = park->pisos[p].id_vehiculos;

            // Iterar las plazas del piso en busca de plazas vacias consecutivas donde aparcar el vehiculo.
            for (int pz = 0; pz < park->n_plazas; pz++)
            {
                cnt = (plazas[pz] == PLAZA_VACIA) ? cnt + 1 : 0;

                // Si se han encontrado tantas plazas consecutivas como el tamano del vehiculo, aparcarlo.
                if (cnt == tamano)
                {
                    // Se actualizan las variables del piso.
                    *p_pz_libres -= tamano;

                    int primera_plaza = pz - tamano + 1;
                    for (int i = primera_plaza; i <= pz; i++)
                    {
                        plazas[i] = matricula;
                    }

                    // Se configura la variable de retorno y se devuelve "true".
                    returns_array[RETURN_PISO] = (unsigned int)p;
                    returns_array[RETURN_PRIMERA_PLAZA] = (unsigned int)primera_plaza;
                    return PLAZA_ENCONTRADA;
                }
            }
        }
    }
    // Si no se encontro donde aparcar el vehiculo, devolver "false".
    return PLAZA_NO_ENCONTRADA;
}

void vaciar_plaza(parking *park, unsigned int tamano, unsigned int piso, unsigned int primera_plaza)
{
    // Se actualizan las variables del piso afectado.
    park->pisos[piso].plazas_libres += tamano;
    int ultima_plaza = primera_plaza + tamano - 1;

    // Iterar las plazas que ocupaba el vehiculo, devolviendolas a su valor por defecto.
    for (int i = primera_plaza; i <= ultima_plaza; i++)
    {
        park->pisos[piso].id_vehiculos[i] = PLAZA_VACIA;
    }
}

void queue_vehiculo(Queue *cola, unsigned int tamano, unsigned int matricula)
{
    printf("Se va a almacenar el vehiculo en la cola\n");
    queue_node *nodo = (queue_node *)malloc(sizeof(queue_node));
    nodo->tamano = tamano;
    nodo->matricula = matricula;
    Enqueue(cola, nodo);
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

void imprimir_cola(const Queue *cola)
{
    printf("Cola: { ");
    queue_node *nodo = cola->head;
    while (nodo != NULL)
    {
        printf("%u ", nodo->matricula);
        nodo = nodo->prev;
    }
    printf("}\n");
}

int main(int argc, char *argv[])
{
    // Se inicializa MPI.
    MPI_Init(&argc, &argv);

    // Se crea el parking.
    parking park;
    int pisos = 0;
    int plazas = 0;
    if (argc == ARGC_NECESARIO)
    {
        pisos = atoi(argv[ARG_PISOS]);
        plazas = atoi(argv[ARG_PLAZAS]);
    }
    pisos = (pisos >= MIN_PISOS) ? pisos : MIN_PISOS;
    plazas = (plazas >= MIN_PLAZAS) ? plazas : MIN_PLAZAS;
    crear_parking(&park, pisos, plazas);

    // Configuracion de las variables de control del bucle principal.
    int n_esclavos = 0; // Numero de procesos coche y camion.
    MPI_Comm_size(MPI_COMM_WORLD, &n_esclavos);
    n_esclavos -= 1; // Se resta el proceso parking.

    Queue *cola_vehiculos = ConstructQueue(n_esclavos);
    unsigned int argumentos[NUM_ARGUMENTOS];
    unsigned int returns_array[NUM_RETURNS];

    while (1)
    {
        // Recibir un mensaje enviado por alguno de los vehiculos.
        MPI_Recv(argumentos, NUM_ARGUMENTOS, MPI_UNSIGNED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Parsear el mensaje.
        unsigned int operacion = argumentos[ARG_OPERACION];
        unsigned int tamano = argumentos[ARG_TAMANO];
        unsigned int matricula_o_piso = argumentos[ARG_MATRICULA_O_PISO];
        unsigned int primera_plaza = argumentos[ARG_PLAZA];

        printf("El parking ha recibido un mensaje. Argumentos:\n");
        printf("\tOperacion: %s\n", (operacion == OP_APARCAR) ? "Aparcar" : "Salir");
        printf("\tTamano del vehiculo: %u\n", tamano);

        // Actuar en funcion del codigo de operacion enviado por el vehiculo.
        switch (operacion)
        {
        case OP_APARCAR:
            printf("\tMatricula del vehiculo: %u\n", matricula_o_piso);

            // Si hay otros vehiculos esperando a entrar, introducir el vehiculo en la cola.
            if (!isEmpty(cola_vehiculos))
            {
                queue_vehiculo(cola_vehiculos, tamano, matricula_o_piso);
            }
            else
            {
                char plaza_encontrada = buscar_plaza(&park, returns_array, matricula_o_piso, tamano);
                printf("Habia plaza para este vehiculo: %u\n", plaza_encontrada);

                // Si se encuentra plaza, notificar al proceso vehiculo
                if (plaza_encontrada)
                {
                    MPI_Send(returns_array, NUM_RETURNS, MPI_UNSIGNED, matricula_o_piso, 0, MPI_COMM_WORLD);
                }
                else
                // Introducirlo en la cola
                {
                    queue_vehiculo(cola_vehiculos, tamano, matricula_o_piso);
                }
            }
            break;
        case OP_SALIR:
            printf("\tPiso en el que el vehiculo esta aparcado: %d\n", matricula_o_piso);
            printf("\tPrimera plaza que ocupa: %d\n", primera_plaza);

            vaciar_plaza(&park, tamano, matricula_o_piso, primera_plaza);

            // Si hay vehiculos en la cola, mirar hay plazas disponibles para su primer elemento.
            if (!isEmpty(cola_vehiculos))
            {
                queue_node *peek = cola_vehiculos->head;

                while (peek != NULL)
                {
                    unsigned int matricula = peek->matricula;
                    unsigned int tamano = peek->tamano;

                    printf("Se va a comprobar si el vehiculo %u de tamano %u tiene sitio en el parking\n", matricula, tamano);

                    char plaza_encontrada = buscar_plaza(&park, returns_array, matricula, tamano);

                    // Si se encontro plaza para el vehiculo, desbloquear al proceso correspondiente.
                    if (plaza_encontrada)
                    {
                        printf("Se encontro en el piso %u. Primera plaza: %u\n",
                               returns_array[RETURN_PISO], returns_array[RETURN_PRIMERA_PLAZA]);
                        queue_node *pop = Dequeue(cola_vehiculos);
                        free(pop);
                        MPI_Send(returns_array, NUM_RETURNS, MPI_UNSIGNED, matricula, 0, MPI_COMM_WORLD);
                        peek = peek->prev;
                    }
                    else
                    {
                        printf("No se encontro\n");
                        peek = NULL;
                    }
                }
            }
            break;
        default:
            fprintf(stderr, "Error: El codigo de operacion {%d} es extrano\n", operacion);
        }
        printf("Parking actualizado:\n");
        imprimir_parking(&park);
        imprimir_cola(cola_vehiculos);
        printf("\n");
    }

    // Se finaliza MPI y se destruyen el parking y la cola.
    MPI_Finalize();
    destruir_parking(&park);
    DestructQueue(cola_vehiculos);

    return 0;
}
