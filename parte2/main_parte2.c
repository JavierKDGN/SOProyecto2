#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funciones.h"  
#include "tabla_hash.h"

int main(int argc, char* argv[]) {
    if (argc != 7) {
        printf("Uso: ./PARTE_2 -m <num_marcos> -a <algoritmo> -f <archivo_referencias>\n");
        return 1;
    }

    int num_marcos = 0;
    char* algoritmo = NULL;
    char* nombre_archivo = NULL;

    // Leer argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0) {
            num_marcos = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-a") == 0) {
            algoritmo = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0) {
            nombre_archivo = argv[++i];
        }
    }

    if (num_marcos <= 0 || algoritmo == NULL || nombre_archivo == NULL) {
        printf("Error en los parámetros de entrada.\n");
        return 1;
    }

    // Leer referencias desde el archivo
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return 1;
    }

    int referencias[1000];
    int num_referencias = 0;
    while (fscanf(archivo, "%d", &referencias[num_referencias]) != EOF) {
        num_referencias++;
    }
    fclose(archivo);

    // Simular el algoritmo
    TablaPaginas* tabla_paginas = tablaInit(SIZE_HASH);
    int fallos_pagina = 0;

    if (strcmp(algoritmo, "FIFO") == 0) {
        for (int i = 0; i < num_referencias; i++) {
            reemplazo_fifo(tabla_paginas, referencias[i], &fallos_pagina, num_marcos);
        }
    } else if (strcmp(algoritmo, "LRU") == 0) {
        for (int i = 0; i < num_referencias; i++) {
            reemplazo_lru(tabla_paginas, referencias[i], &fallos_pagina, num_marcos);
        }
    } else if (strcmp(algoritmo, "OPTIMO") == 0) {
        for (int i = 0; i < num_referencias; i++) {
            reemplazo_optimo(tabla_paginas, referencias, num_referencias, i, &fallos_pagina, num_marcos);
        }
    } else if (strcmp(algoritmo, "RELOJ_SIMPLE") == 0) {
        int referencia[num_marcos];
        int puntero = 0;
        memset(referencia, 0, sizeof(referencia)); // Inicializar bits de referencia
        for (int i = 0; i < num_referencias; i++) {
            reemplazo_reloj_simple(tabla_paginas, referencias[i], referencia, &puntero, &fallos_pagina, num_marcos);
        }
    } else {
        printf("Algoritmo no implementado: %s\n", algoritmo);
        liberar_tabla_paginas(tabla_paginas);
        return 1;
    }

    printf("Total de fallos de página: %d\n", fallos_pagina);

    tablaFree(tabla_paginas);
    return 0;
}
