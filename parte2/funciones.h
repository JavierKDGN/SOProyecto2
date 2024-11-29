#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "tabla_hash.h"

int FIFO(int* marcos, int num_marcos, int* referencias, int num_referencias);
int LRU(int* marcos, int num_marcos, int* referencias, int num_referencias);
int OPTIMO(int* marcos, int num_marcos, int* referencias, int num_referencias);
int RELOJ_SIMPLE(int* marcos, int num_marcos, int* referencias, int num_referencias);
void imprimir_marcos(int* marcos, int num_marcos);

#endif
