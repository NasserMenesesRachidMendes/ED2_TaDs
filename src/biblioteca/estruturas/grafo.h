/*
 * Trabalho Pratico 1 - Estruturas de Dados 2
 * Aluno(s): PREENCHER NOME(S)
 * Matricula(s): PREENCHER MATRICULA(S)
 * Data: PREENCHER DATA
 */
#ifndef GRAFO_H
#define GRAFO_H

typedef struct XXgrafoXX *Grafo;

Grafo GGcriaGrafo(int nv, int na);
void GGdestroiGrafo(Grafo g);
int GVcriaVertice(Grafo g);
int GAcriaAresta(Grafo g, int v1, int v2);

int GBexisteIdVertice(Grafo g, int v);
int GBexisteIdAresta(Grafo g, int a);
int GBexisteArestaDir(Grafo g, int v1, int v2);
int GBexisteAresta(Grafo g, int v1, int v2);
int GApegaArestaDir(Grafo g, int v1, int v2);
int GApegaAresta(Grafo g, int v1, int v2);

int GVprimeiroVertice(Grafo g);
int GVproximoVertice(Grafo g, int v1);
int GAprimeiraAresta(Grafo g);
int GAproximaAresta(Grafo g, int a1);

int GInumeroVertices(Grafo g);
int GInumeroVerticesMax(Grafo g);
int GInumeroArestas(Grafo g);
int GInumeroArestasMax(Grafo g);

Grafo GGcarregaGrafo(const char *arquivo);
int GBsalvaGrafo(Grafo g, const char *arquivo);

int GIpegaGrau(Grafo g, int v);
int GAprimaAresta(Grafo g, int v);
int GAproxAresta(Grafo g, int v, int a1);
int GAprimaEntrada(Grafo g, int v);
int GAproxEntrada(Grafo g, int v, int a1);
int GAprimaSaida(Grafo g, int v);
int GAproxSaida(Grafo g, int v, int a1);

int GBarestaLaco(Grafo g, int a);
int GValfa(Grafo g, int a);
int GVomega(Grafo g, int a);
int GVvizinho(Grafo g, int a, int v1);

#endif
