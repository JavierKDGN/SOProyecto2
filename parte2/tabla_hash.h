#ifndef TABLA_HASH_H

#define SIZE_HASH 11

typedef struct NodoPagina {
    int num_pagina;
    int num_marco;
    struct NodoPagina* next;
} NodoPagina;

typedef struct TablaPaginas {
    int size;
    NodoPagina** tabla;
} TablaPaginas;

TablaPaginas* tablaInit(int size);
void tablaFree(TablaPaginas* tabla);
void insertarPagina(TablaPaginas* tabla, int num_pagina, int num_marco);
void eliminarPagina(TablaPaginas* tabla, int num_pagina);
int buscarPagina(TablaPaginas* tabla, int num_pagina);
int hashFunction(int num_pagina, int size_tabla);

#endif 