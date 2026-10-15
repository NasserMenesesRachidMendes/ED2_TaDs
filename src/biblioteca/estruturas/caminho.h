/*
 * Trabalho Pratico 1 - Estruturas de Dados 2
 * Aluno(s): PREENCHER NOME(S)
 * Matricula(s): PREENCHER MATRICULA(S)
 * Data: PREENCHER DATA
 */
#ifndef CAMINHO_H
#define CAMINHO_H

#include "biblioteca/estruturas/grafo.h"

/* Imprime o caminho de menor custo dirigido de a ate b.
   pesos[1..|A|] guarda os custos; pesos[0] nao e utilizada.
   Se nao houver caminho, imprime somente a. Os pesos devem ser >= 0. */
void Gcaminho(Grafo g, float *pesos, int a, int b);

#endif
