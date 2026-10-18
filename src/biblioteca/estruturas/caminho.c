/*
 *
 * Dijkstra usando as operacoes publicas da TAD Grafo. O algoritmo exige
 * pesos nao negativos. O vetor de pesos e indexado pelo id da aresta.
 */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "biblioteca/estruturas/caminho.h"

void Gcaminho(Grafo g, float *pesos, int a, int b) {
    int n, i, v, u, e, escolhido, k;
    float *dist;
    int *anterior, *visitado, *caminho;

    if (!g || !pesos || !GBexisteIdVertice(g,a) || !GBexisteIdVertice(g,b)) {
        if (a > 0) printf("%d", a);
        else printf("0");
        printf("\n");
        return;
    }
    if (a == b) { printf("%d\n", a); return; }

    /* Validar todos os pesos: Dijkstra nao e correto com pesos negativos,
       mesmo que a aresta negativa nao seja visitada durante a busca. */
    for (e = GAprimeiraAresta(g); e != 0; e = GAproximaAresta(g, e)) {
        if (pesos[e] < 0.0f) { printf("%d\n", a); return; }
    }

    n = GInumeroVertices(g);
    dist = (float *)malloc(((size_t)n + 1) * sizeof(*dist));
    anterior = (int *)calloc((size_t)n + 1, sizeof(*anterior));
    visitado = (int *)calloc((size_t)n + 1, sizeof(*visitado));
    caminho = (int *)malloc(((size_t)n + 1) * sizeof(*caminho));
    if (!dist || !anterior || !visitado || !caminho) {
        free(dist); free(anterior); free(visitado); free(caminho);
        printf("%d\n", a);
        return;
    }

    for (v = 1; v <= n; ++v) dist[v] = FLT_MAX;
    dist[a] = 0.0f;

    for (i = 0; i < n; ++i) {
        float menor = FLT_MAX;
        escolhido = 0;
        for (v = 1; v <= n; ++v) {
            if (!visitado[v] && dist[v] < menor) {
                menor = dist[v];
                escolhido = v;
            }
        }
        if (escolhido == 0 || escolhido == b) break;
        visitado[escolhido] = 1;

        for (e = GAprimaSaida(g, escolhido); e != 0;
             e = GAproxSaida(g, escolhido, e)) {
            u = GVomega(g, e);
            if (u < 1 || u > n || visitado[u]) continue;
            if (dist[escolhido] != FLT_MAX &&
                dist[escolhido] + pesos[e] < dist[u]) {
                dist[u] = dist[escolhido] + pesos[e];
                anterior[u] = escolhido;
            }
        }
    }

    if (dist[b] == FLT_MAX) {
        printf("%d", a);
    } else {
        k = 0;
        for (v = b; v != 0 && k <= n; v = anterior[v]) {
            caminho[k++] = v;
            if (v == a) break;
        }
        if (k == 0 || caminho[k - 1] != a) {
            printf("%d", a);
        } else {
            for (i = k - 1; i >= 0; --i)
                printf("%s%d", i == k - 1 ? "" : " -> ", caminho[i]);
        }
    }

    printf("\n");
    free(dist);
    free(anterior);
    free(visitado);
    free(caminho);
}
