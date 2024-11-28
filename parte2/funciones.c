#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "funciones.h"

// Inicializar la tabla de páginas
void inicializar_tabla_paginas(TablaPaginas* tabla_paginas) {
    for (int i = 0; i < TAMANO_HASH; i++) {
        tabla_paginas->tabla[i] = NULL;
    }
}

// funcion hash
int funcion_hash(int numero_pagina) {
    return numero_pagina % TAMANO_HASH;
}

// Buscar pagina en la tabla
bool buscar_pagina(TablaPaginas* tabla_paginas, int numero_pagina) {
    int indice = funcion_hash(numero_pagina);
    NodoPagina* actual = tabla_paginas->tabla[indice];
    while (actual != NULL) {
        if (actual->numero_pagina == numero_pagina) {
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

// Insertar pagina en la tabla
void insertar_pagina(TablaPaginas* tabla_paginas, int numero_pagina) {
    int indice = funcion_hash(numero_pagina);
    NodoPagina* nuevo_nodo = (NodoPagina*)malloc(sizeof(NodoPagina));
    nuevo_nodo->numero_pagina = numero_pagina;
    nuevo_nodo->siguiente = tabla_paginas->tabla[indice];
    tabla_paginas->tabla[indice] = nuevo_nodo;
}

// Eliminar pagina de la tabla
void eliminar_pagina(TablaPaginas* tabla_paginas, int numero_pagina) {
    int indice = funcion_hash(numero_pagina);
    NodoPagina* actual = tabla_paginas->tabla[indice];
    NodoPagina* previo = NULL;
    while (actual != NULL && actual->numero_pagina != numero_pagina) {
        previo = actual;
        actual = actual->siguiente;
    }
    if (actual != NULL) {
        if (previo == NULL) {
            tabla_paginas->tabla[indice] = actual->siguiente;
        } else {
            previo->siguiente = actual->siguiente;
        }
        free(actual);
    }
}


int OPTIMO (int* marcos, int num_marcos, int* referencias, int num_referencias) {
    int fallos_pagina = 0;

    // Inicializar los marcos como vacios
    for (int i = 0; i < num_marcos; i++) {
        marcos[i] = -1;
    }

    for (int i = 0; i < num_referencias; i++) {
        bool encontrado = false;

        // Verificar si la página ya esta en memoria
        for (int j = 0; j < num_marcos; j++) {
            if (marcos[j] == referencias[i]) {
                encontrado = true;
                break;
            }
        }

        // Si no esta, es un fallo de pagina
        if (!encontrado) {
            fallos_pagina++;

            bool marco_asignado = false;
            // Si hay espacio, encontrar el primer marco vacio
            for (int j = 0; j < num_marcos; j++) {
                if (marcos[j] == -1) {
                    marcos[j] = referencias[i];
                    marco_asignado = true;
                    break;
                }
            }

            // Si no hay espacio, aplicar el reemplazo optimo
            if (!marco_asignado) {
                int indice_remplazar = -1;
                int max_distancia = -1;

                for (int j = num_marcos - 1; j >= 0; j--) {
                    int distancia = -1;

                    // Buscar cuándo se usara de nuevo cada pagina en el futuro
                    for (int k = i + 1; k < num_referencias; k++) {
                        if (marcos[j] == referencias[k]) {
                            distancia = k - i;
                            break;
                        }
                    }

                    // Si no se usara en el futuro, reemplazar esta pagina
                    if (distancia == -1) {
                        indice_remplazar = j;
                        break;
                    }

                    // Actualizar la pagina con la mayor distancia
                    if (distancia > max_distancia) {
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
int FIFO(int* marcos, int num_marcos, int* referencias, int num_referencias) {
    int fallos_pagina = 0;         // Contador de fallos de página
    //int siguiente_marco = 0;      // Indica el próximo marco a reemplazar

    // Inicializar los marcos como vacíos (-1)
    for (int i = 0; i < num_marcos; i++) {
        marcos[i] = -1;
    }

    // Iterar por cada referencia
    for (int i = 0; i < num_referencias; i++) {
        bool encontrado = false;  // Indica si la referencia ya está en memoria

        // Verificar si la página ya está en los marcos
        for (int j = 0; j < num_marcos; j++) {
            if (marcos[j] == referencias[i]) {
                encontrado = true;
                break;  // Salir del bucle si la página ya está cargada
            }
        }

        // Si no está, hay un fallo de página
        if (!encontrado) {
            fallos_pagina++;  // Incrementar el contador de fallos
            for (int j = 0; j < num_marcos - 1; j++)
            {
                marcos[j] = marcos[j + 1];
            }
            
            marcos[num_marcos - 1] = referencias[i];  // Reemplazar en el marco FIFO
            //siguiente_marco = (siguiente_marco + 1) % num_marcos;  // Avanzar al siguiente marco
        }

        // Imprimir el estado actual de los marcos
        imprimir_marcos(marcos, num_marcos);
    }

    return fallos_pagina;
}



int RELOJ_SIMPLE (int* marcos, int num_marcos, int* referencias, int num_referencias) {
    int fallos_pagina = 0;
    int* bits_referencia = (int*)malloc(num_marcos * sizeof(int)); // Bits de referencia
    int puntero_reloj = 0;           // Puntero al marco actual

    // Inicializar los marcos y bits de referencia
    for (int i = 0; i < num_marcos; i++) {
        marcos[i] = -1;
        bits_referencia[i] = 0;
    }

    for (int i = 0; i < num_referencias; i++) {
        bool encontrado = false;

        // Verificar si la pagina ya esta en memoria
        for (int j = 0; j < num_marcos; j++) {
            if (marcos[j] == referencias[i]) {
                encontrado = true;
                bits_referencia[j] = 1; // Actualizar el bit de referencia
                break;
            }
        }

        // Si no esta, es un fallo de pagina
        if (!encontrado) {
            fallos_pagina++;

            while (true) {
                // Si el bit de referencia es 0, reemplazar esta pagina
                if (bits_referencia[puntero_reloj] == 0) {
                    marcos[puntero_reloj] = referencias[i];
                    bits_referencia[puntero_reloj] = 1; // Activar el bit de referencia
                    puntero_reloj = (puntero_reloj + 1) % num_marcos;
                    break;
                }

                // Si el bit de referencia es 1, resetear y avanzar
                bits_referencia[puntero_reloj] = 0;
                puntero_reloj = (puntero_reloj + 1) % num_marcos;
            }
        }

        imprimir_marcos(marcos, num_marcos);
    }

    free(bits_referencia);
    return fallos_pagina;
}


int LRU(int* marcos, int num_marcos, int* referencias, int num_referencias) {
    int fallos_pagina = 0;
    //int tiempos[num_marcos]; // Para rastrear el tiempo relativo de uso

    // Inicializar los marcos y los tiempos
    for (int i = 0; i < num_marcos; i++) {
        marcos[i] = -1; // Marcos vacíos inicialmente
        //tiempos[i] = -1; // -1 indica que no se ha usado aún
    }

    for (int i = 0; i < num_referencias; i++) {
        bool encontrado = false;

        // Incrementar los tiempos para todos los marcos ocupados
        //for (int j = 0; j < num_marcos; j++) {
        //    if (marcos[j] != -1) {
        //        tiempos[j]++;
        //    }
        //}

        // Verificar si la página ya está en memoria
        for (int j = 0; j < num_marcos; j++) {
            if (marcos[j] == referencias[i]) {
                encontrado = true;
                for (int k = 0; k < num_marcos - 1; k++)
                {
                    marcos[k] = marcos[k + 1];
                }
                marcos[num_marcos - 1] = referencias[i];
                //tiempos[j] = 0; // Reiniciar el tiempo relativo de esta página
                break;
            }
        }

        // Si no está, es un fallo de página
        if (!encontrado) {
            fallos_pagina++;  // Incrementar el contador de fallos
            for (int j = 0; j < num_marcos - 1; j++)
            {
                marcos[j] = marcos[j + 1];
            }
            
            marcos[num_marcos - 1] = referencias[i];  // Reemplazar en el marco FIFO
        }

        // Imprimir el estado actual de los marcos
        imprimir_marcos(marcos, num_marcos);
    }

    return fallos_pagina;
}


// Imprimir los marcos
void imprimir_marcos(int* marcos, int num_marcos) {
    printf("Marcos: ");
    for (int i = 0; i < num_marcos; i++) {
        if (marcos[i] == -1) {
            printf("[ ] ");
        } else {
            printf("[%d] ", marcos[i]);
        }
    }
    printf("\n");
}
