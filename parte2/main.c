#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funciones.h"

int main(int argc, char *argv[])
{
    if (argc != 7)
    {
        printf("Uso: ./PARTE_2 -m <num_marcos> -a <algoritmo> -f <archivo_referencias>\n"); // forma de uso
        return 1;
    }

    int num_marcos = 0;     // numero de marcos
    char *algoritmo = NULL; // algoritmo
    char *nombre_archivo = NULL;

    // Leer argumentos de la linea de comandos
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-m") == 0)
        {
            num_marcos = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            algoritmo = argv[++i];
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            nombre_archivo = argv[++i];
        }
    }
    // Verificar que los parametros de entrada sean validos
    if (num_marcos <= 0 || algoritmo == NULL || nombre_archivo == NULL)
    {
        printf("Error en los parametros de entrada.\n");
        return 1;
    }

    // Leer referencias desde el archivo
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return 1;
    }

    int referencias[1000];
    int num_referencias = 0;
    while (fscanf(archivo, "%d", &referencias[num_referencias]) != EOF)
    {
        num_referencias++;
    }
    fclose(archivo);

    // Simular el algoritmo
    int fallos_pagina = 0;
    int *marcos = (int *)malloc(num_marcos * sizeof(int));

    if (strcmp(algoritmo, "FIFO") == 0) // comparar si el algoritmo es FIFO
    {
        fallos_pagina = FIFO(marcos, num_marcos, referencias, num_referencias); // llamar a la funcion FIFO
    }
    else if (strcmp(algoritmo, "LRU") == 0)
    {
        fallos_pagina = LRU(marcos, num_marcos, referencias, num_referencias); // llamar a la funcion LRU
    }
    else if (strcmp(algoritmo, "OPTIMO") == 0)
    {
        fallos_pagina = OPTIMO(marcos, num_marcos, referencias, num_referencias); // llamar a la funcion OPTIMO
    }
    else if (strcmp(algoritmo, "RELOJ_SIMPLE") == 0)
    {
        fallos_pagina = RELOJ_SIMPLE(marcos, num_marcos, referencias, num_referencias); // llamar a la funcion RELOJ_SIMPLE
    }
    else
    {
        printf("Algoritmo no implementado: %s\n", algoritmo); // si el algoritmo no esta implementado
        free(marcos);                                         // Liberar memoria
        return 1;
    }

    printf("Total de fallos de pagina: %d\n", fallos_pagina); // imprimir el total de fallos de pagina

    free(marcos); // Liberar memoria
    return 0;
}
