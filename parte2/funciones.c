#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "funciones.h"

// Inicializar la tabla de páginas
void inicializar_tabla_paginas(TablaPaginas *tabla_paginas)
{
    for (int i = 0; i < TAMANO_HASH; i++)
    {
        tabla_paginas->tabla[i] = NULL;
    }
}

// funcion hash
int funcion_hash(int numero_pagina)
{
    return numero_pagina % TAMANO_HASH;
}

// Buscar pagina en la tabla
bool buscar_pagina(TablaPaginas *tabla_paginas, int numero_pagina)
{
    int indice = funcion_hash(numero_pagina);
    NodoPagina *actual = tabla_paginas->tabla[indice];
    while (actual != NULL)
    {
        if (actual->numero_pagina == numero_pagina)
        {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

// Insertar pagina en la tabla
void insertar_pagina(TablaPaginas *tabla_paginas, int numero_pagina)
{
    int indice = funcion_hash(numero_pagina);
    NodoPagina *nuevo_nodo = (NodoPagina *)malloc(sizeof(NodoPagina));
    nuevo_nodo->numero_pagina = numero_pagina;
    nuevo_nodo->siguiente = tabla_paginas->tabla[indice];
    tabla_paginas->tabla[indice] = nuevo_nodo;
}

// Eliminar pagina de la tabla
void eliminar_pagina(TablaPaginas *tabla_paginas, int numero_pagina)
{
    int indice = funcion_hash(numero_pagina);
    NodoPagina *actual = tabla_paginas->tabla[indice];
    NodoPagina *previo = NULL;
    while (actual != NULL && actual->numero_pagina != numero_pagina)
    {
        previo = actual;
        actual = actual->siguiente;
    }
    if (actual != NULL)
    {
        if (previo == NULL)
        {
            tabla_paginas->tabla[indice] = actual->siguiente;
        }
        else
        {
            previo->siguiente = actual->siguiente;
        }
        free(actual);
    }
}

int OPTIMO(int *marcos, int num_marcos, int *referencias, int num_referencias)
{
    int fallos_pagina = 0;

    // Inicializar los marcos como vacios
    for (int i = 0; i < num_marcos; i++)
    {
        marcos[i] = -1;
    }

    for (int i = 0; i < num_referencias; i++)
    {
        bool encontrado = false;

        // Verificar si la página ya esta en memoria
        for (int j = 0; j < num_marcos; j++)
        {
            if (marcos[j] == referencias[i])
            {
                encontrado = true;
                break;
            }
        }

        // Si no esta, es un fallo de pagina
        if (!encontrado)
        {
            fallos_pagina++;

            bool marco_asignado = false;
            // Si hay espacio, encontrar el primer marco vacio
            for (int j = 0; j < num_marcos; j++)
            {
                if (marcos[j] == -1)
                {
                    marcos[j] = referencias[i];
                    marco_asignado = true;
                    break;
                }
            }

            // Si no hay espacio, aplicar el reemplazo optimo
            if (!marco_asignado)
            {
                int indice_remplazar = -1;
                int max_distancia = -1;

                for (int j = num_marcos - 1; j >= 0; j--)
                {
                    int distancia = -1;

                    // Buscar cuándo se usara de nuevo cada pagina en el futuro
                    for (int k = i + 1; k < num_referencias; k++)
                    {
                        if (marcos[j] == referencias[k])
                        {
                            distancia = k - i;
                            break;
                        }
                    }

                    // Si no se usara en el futuro, reemplazar esta pagina
                    if (distancia == -1)
                    {
                        indice_remplazar = j;
                        break;
                    }

                    // Actualizar la pagina con la mayor distancia
                    if (distancia > max_distancia)
                    {
                        max_distancia = distancia;
                        indice_remplazar = j;
                    }
                }

                marcos[indice_remplazar] = referencias[i];
            }
        }

        imprimir_marcos(marcos, num_marcos);
    }

    return fallos_pagina;
}

// Algoritmo FIFO
int FIFO(int *marcos, int num_marcos, int *referencias, int num_referencias)
{
    int fallos_pagina = 0; // Contador de fallos de página
    // int siguiente_marco = 0;      // Indica el próximo marco a reemplazar

    // Inicializar los marcos como vacíos (-1)
    for (int i = 0; i < num_marcos; i++)
    {
        marcos[i] = -1;
    }

    // Iterar por cada referencia
    for (int i = 0; i < num_referencias; i++)
    {
        bool encontrado = false; // Indica si la referencia ya está en memoria

        // Verificar si la página ya está en los marcos
        for (int j = 0; j < num_marcos; j++)
        {
            if (marcos[j] == referencias[i])
            {
                encontrado = true;
                break; // Salir del bucle si la página ya está cargada
            }
        }

        // Si no está, hay un fallo de página
        if (!encontrado)
        {
            fallos_pagina++; // Incrementar el contador de fallos
            for (int j = 0; j < num_marcos - 1; j++)
            {
                marcos[j] = marcos[j + 1];
            }

            marcos[num_marcos - 1] = referencias[i]; // Reemplazar en el marco FIFO
            // siguiente_marco = (siguiente_marco + 1) % num_marcos;  // Avanzar al siguiente marco
        }

        // Imprimir el estado actual de los marcos
        imprimir_marcos(marcos, num_marcos);
    }

    return fallos_pagina;
}

int RELOJ_SIMPLE(int *marcos, int num_marcos, int *referencias, int num_referencias)
{
    int fallos_pagina = 0;
    bool *bits_referencia = (bool *)malloc(num_marcos * sizeof(bool)); // Arreglo con memoria dinamica
    int puntero_reloj = 0;

    // Inicializar los marcos y los bits de referencia
    for (int i = 0; i < num_marcos; i++)
    {
        marcos[i] = -1;
        bits_referencia[i] = false;
    }

    for (int i = 0; i < num_referencias; i++)
    {
        bool encontrado = false;

        // Verificar si la pagina ya esta en los marcos
        for (int j = 0; j < num_marcos; j++)
        {
            if (marcos[j] == referencias[i])
            {
                encontrado = true;
                bits_referencia[j] = true; // marcamos como usada recentiemente
                break;
            }
        }

        // Si no esta, es un fallo de página
        if (!encontrado)
        {
            fallos_pagina++;

            // reemplazamos segun bit de referencia
            while (true)
            {
                if (!bits_referencia[puntero_reloj])
                {
                    // Reemplazar si el bit de referencia es 0
                    marcos[puntero_reloj] = referencias[i];
                    bits_referencia[puntero_reloj] = true; // Establecer el bit de referencia
                    puntero_reloj = (puntero_reloj + 1) % num_marcos;
                    break;
                }
                else
                {
                    // Resetear el bit si es 1 y avanzar el puntero
                    bits_referencia[puntero_reloj] = false;
                    puntero_reloj = (puntero_reloj + 1) % num_marcos;
                }
            }
        }

        // Imprimir el estado actual de los marcos
        imprimir_marcos(marcos, num_marcos);
    }

    free(bits_referencia); // Liberar memoria
    return fallos_pagina;
}

int LRU(int *marcos, int num_marcos, int *referencias, int num_referencias)
{
    int fallos_pagina = 0;

    // Inicializar los marcos como vacios
    for (int i = 0; i < num_marcos; i++)
    {
        marcos[i] = -1;
    }

    for (int i = 0; i < num_referencias; i++)
    {
        bool encontrado = false;
        int pos_encontrada = -1;

        // Verificar si la pagina ya esta en memoria
        for (int j = 0; j < num_marcos; j++)
        {
            if (marcos[j] == referencias[i])
            {
                encontrado = true;
                pos_encontrada = j;
                break;
            }
        }

        // si esta, mover la pagina encontrada al final para simular el uso reciente
        if (encontrado)
        {
            int pagina_actual = marcos[pos_encontrada];
            for (int j = pos_encontrada; j < num_marcos - 1; j++)
            {
                marcos[j] = marcos[j + 1];
            }
            marcos[num_marcos - 1] = pagina_actual;
        }

        // si no es fallo de pagina
        if (!encontrado)
        {
            fallos_pagina++;

            // mover las paginas hacia la izquierda para hacer espacio al nuevo al final
            for (int j = 0; j < num_marcos - 1; j++)
            {
                marcos[j] = marcos[j + 1];
            }

            // Insertar la nueva pagina al final
            marcos[num_marcos - 1] = referencias[i];
        }

        // Imprimir el estado actual de los marcos
        imprimir_marcos(marcos, num_marcos);
    }

    return fallos_pagina;
}

// Imprimir los marcos
void imprimir_marcos(int *marcos, int num_marcos)
{
    printf("Marcos: ");
    for (int i = 0; i < num_marcos; i++)
    {
        if (marcos[i] == -1)
        {
            printf("[ ] ");
        }
        else
        {
            printf("[%d] ", marcos[i]);
        }
    }
    printf("\n");
}
