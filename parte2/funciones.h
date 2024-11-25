#ifndef FUNCIONES_H
#define FUNCIONES_H

#define TAMANO_HASH 10

// Declaraciones de estructuras
typedef struct NodoPagina {
    int numero_pagina;
    struct NodoPagina* siguiente;
} NodoPagina;

typedef struct TablaPaginas {
    NodoPagina* tabla[TAMANO_HASH];
} TablaPaginas;

// Declaraciones de funciones
void inicializar_tabla_paginas(TablaPaginas* tabla_paginas);
int funcion_hash(int numero_pagina);
bool buscar_pagina(TablaPaginas* tabla_paginas, int numero_pagina);
void insertar_pagina(TablaPaginas* tabla_paginas, int numero_pagina);
void eliminar_pagina(TablaPaginas* tabla_paginas, int numero_pagina);
int FIFO(int* marcos, int num_marcos, int* referencias, int num_referencias);
int LRU(int* marcos, int num_marcos, int* referencias, int num_referencias);
int OPTIMO(int* marcos, int num_marcos, int* referencias, int num_referencias);
int RELOJ_SIMPLE(int* marcos, int num_marcos, int* referencias, int num_referencias);
void imprimir_marcos(int* marcos, int num_marcos);

#endif
