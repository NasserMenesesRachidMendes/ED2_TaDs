#include <assert.h>
#include <stdio.h>
#include "biblioteca/estruturas/grafo.h"

int main(void) {
    Grafo g = GGcriaGrafo(3, 3);
    int a1, a2, a3;
    FILE *f;

    assert(g != NULL);
    assert(GInumeroVertices(g) == 0);
    assert(GInumeroArestas(g) == 0);
    assert(GVcriaVertice(g) == 1);
    assert(GVcriaVertice(g) == 2);
    assert(GVcriaVertice(g) == 3);
    assert(GVcriaVertice(g) == 0); /* capacidade esgotada */

    a1 = GAcriaAresta(g, 1, 2);
    a2 = GAcriaAresta(g, 2, 3);
    a3 = GAcriaAresta(g, 1, 1); /* laco */
    assert(a1 == 1 && a2 == 2 && a3 == 3);
    assert(GAcriaAresta(g, 3, 1) == 0); /* capacidade esgotada */

    assert(GBexisteArestaDir(g, 1, 2));
    assert(!GBexisteArestaDir(g, 2, 1));
    assert(GBexisteAresta(g, 2, 1));
    assert(GApegaArestaDir(g, 2, 3) == 2);
    assert(GApegaArestaDir(g, 3, 2) == 0);
    assert(GApegaAresta(g, 3, 2) == 2);
    assert(GIpegaGrau(g, 1) == 3); /* aresta 1 + laco contado duas vezes */
    assert(GBarestaLaco(g, 3));
    assert(GValfa(g, 2) == 2 && GVomega(g, 2) == 3);
    assert(GVvizinho(g, 2, 3) == 2);
    assert(GVvizinho(g, 2, 1) == 0);

    assert(GBsalvaGrafo(g, "build/test_grafo.txt"));
    GGdestroiGrafo(g);
    g = GGcarregaGrafo("build/test_grafo.txt");
    assert(g != NULL);
    assert(GInumeroVertices(g) == 3 && GInumeroArestas(g) == 3);
    GGdestroiGrafo(g);

    /* Arquivo valido contendo vertices, mas nenhuma aresta. */
    f = fopen("build/test_grafo_vazio.txt", "w");
    assert(f != NULL);
    fputs("3 0\n", f);
    fclose(f);
    g = GGcarregaGrafo("build/test_grafo_vazio.txt");
    assert(g != NULL);
    assert(GInumeroVertices(g) == 3 && GInumeroArestas(g) == 0);
    GGdestroiGrafo(g);

    puts("Todos os testes automatizados da TAD Grafo passaram.");
    return 0;
}
