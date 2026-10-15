#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "biblioteca/estruturas/grafo.h"

/* Os algoritmos deste trabalho interpretam cada aresta como nao dirigida.
   Os pesos sao indexados pelo identificador da aresta: pesos[1..|E|]. */
void AGM(Grafo g, const float *pesos);
void BEL(Grafo g, int origem);
void BEP(Grafo g, int origem);
void CMC(Grafo g, const float *pesos, int origem, int destino);
void CC(Grafo g);

#endif
