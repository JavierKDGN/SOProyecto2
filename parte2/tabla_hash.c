#include "tabla_hash.h"
#include <stdlib.h>


TablaPaginas* tablaInit(int size) {
    TablaPaginas* tabla_pagina = malloc(sizeof(TablaPaginas));
    tabla_pagina->size = size;
    tabla_pagina->tabla = malloc(size * sizeof(NodoPagina*));
    for (int i = 0; i < size; i++) {
        tabla_pagina->tabla[i] = NULL;
    }
    return tabla_pagina;
}

void tablaFree(TablaPaginas* tabla) {
    for (int i = 0; i < tabla->size; i++) {
        NodoPagina* actual = tabla->tabla[i];
        while (actual) {
            NodoPagina* temp = actual;
            actual = actual->next;
            free(temp);
        }
    }
    free(tabla->tabla);
    free(tabla);
}

int hashFunction(int num_pagina, int size_tabla) {
    return num_pagina % size_tabla;
}

void insertarPagina(TablaPaginas* tabla, int num_pagina, int num_marco) {
    int index = hashFunction(num_pagina, tabla->size);
    NodoPagina* nueva_pagina = malloc(sizeof(NodoPagina));
    nueva_pagina->num_pagina = num_pagina;
    nueva_pagina->num_marco = num_marco;
    nueva_pagina->next = tabla->tabla[index];
    tabla->tabla[index] = nueva_pagina;
}

void eliminarPagina(TablaPaginas* tabla, int num_pagina) {
    int index = hashFunction(num_pagina, tabla->size);
    NodoPagina* actual = tabla->tabla[index];
    NodoPagina* anterior = NULL;
    while (actual != NULL) {
        if (actual->num_pagina == num_pagina) {
            if (anterior == NULL) {
                tabla->tabla[index] = actual->next;
            } else {
                anterior->next = actual->next;
            }
            free(actual);
            return;
        }
        anterior = actual;
        actual = actual->next;
    }
}

int buscarPagina(TablaPaginas* tabla, int num_pagina) {
    int index = hashFunction(num_pagina, tabla->size);
    NodoPagina* actual = tabla->tabla[index];
    while (actual != NULL) {
        if (actual->num_pagina == num_pagina) {
            return actual->num_marco;
        }
        actual = actual->next;
    }
    return -1;
}