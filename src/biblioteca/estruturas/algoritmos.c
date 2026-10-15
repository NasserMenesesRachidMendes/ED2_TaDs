#include "biblioteca/estruturas/algoritmos.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { int id; float peso; } ItemAresta;

static int comparaArestas(const void *pa, const void *pb) {
    const ItemAresta *a = (const ItemAresta *)pa;
    const ItemAresta *b = (const ItemAresta *)pb;
    if (a->peso < b->peso) return -1;
    if (a->peso > b->peso) return 1;
    return (a->id > b->id) - (a->id < b->id);
}

static int raiz(int *pai, int x) {
    while (pai[x] != x) {
        pai[x] = pai[pai[x]];
        x = pai[x];
    }
    return x;
}

static void une(int *pai, int *rank, int a, int b) {
    a = raiz(pai, a); b = raiz(pai, b);
    if (a == b) return;
    if (rank[a] < rank[b]) pai[a] = b;
    else if (rank[a] > rank[b]) pai[b] = a;
    else { pai[b] = a; rank[a]++; }
}

void AGM(Grafo g, const float *pesos) {
    int n, m, i, a, escolhidas = 0;
    ItemAresta *ordem;
    int *pai, *rank;
    double custo = 0.0;
    if (!g || !pesos) { puts("Erro: grafo ou pesos invalidos."); return; }
    n = GInumeroVertices(g); m = GInumeroArestas(g);
    ordem = (ItemAresta *)malloc((size_t)(m ? m : 1) * sizeof(*ordem));
    pai = (int *)malloc(((size_t)n + 1) * sizeof(*pai));
    rank = (int *)calloc((size_t)n + 1, sizeof(*rank));
    if (!ordem || !pai || !rank) {
        free(ordem); free(pai); free(rank); puts("Erro: memoria insuficiente."); return;
    }
    for (i = 1; i <= n; ++i) pai[i] = i;
    for (a = 1; a <= m; ++a) { ordem[a-1].id = a; ordem[a-1].peso = pesos[a]; }
    qsort(ordem, (size_t)m, sizeof(*ordem), comparaArestas);
    printf("Arestas da AGM (Kruskal): ");
    for (i = 0; i < m && escolhidas < n - 1; ++i) {
        int id = ordem[i].id, u = GValfa(g,id), v = GVomega(g,id);
        if (u != v && raiz(pai,u) != raiz(pai,v)) {
            une(pai,rank,u,v);
            printf("a%d(v%d-v%d, peso=%.3g) ", id, u, v, pesos[id]);
            custo += pesos[id]; escolhidas++;
        }
    }
    if (n <= 1) printf("(nenhuma aresta necessaria) ");
    if (escolhidas < n - 1) printf("\nAviso: grafo desconexo; foi encontrada uma floresta geradora minima.");
    printf("\nCusto total: %.6g\n", custo);
    free(ordem); free(pai); free(rank);
}

void BEL(Grafo g, int origem) {
    int n, inicio = 0, fim = 0, v, e, w;
    int *fila, *visitado;
    if (!g || !GBexisteIdVertice(g, origem)) { puts("Vertice de partida invalido."); return; }
    n = GInumeroVertices(g);
    fila = (int *)malloc(((size_t)n + 1) * sizeof(*fila));
    visitado = (int *)calloc((size_t)n + 1, sizeof(*visitado));
    if (!fila || !visitado) { free(fila); free(visitado); puts("Erro: memoria insuficiente."); return; }
    printf("Historico BEL: v%d", origem);
    visitado[origem] = 1; fila[fim++] = origem;
    while (inicio < fim) {
        v = fila[inicio++];
        for (e = GAprimaAresta(g,v); e; e = GAproxAresta(g,v,e)) {
            w = GVvizinho(g,e,v);
            if (w && !visitado[w]) {
                visitado[w] = 1; fila[fim++] = w;
                printf(",a%d,v%d", e, w);
            }
        }
    }
    puts("");
    free(fila); free(visitado);
}

static void BEPrecursiva(Grafo g, int v, int *visitado) {
    int e, w;
    visitado[v] = 1;
    for (e = GAprimaAresta(g,v); e; e = GAproxAresta(g,v,e)) {
        w = GVvizinho(g,e,v);
        if (w && !visitado[w]) {
            printf(",a%d,v%d", e, w);
            BEPrecursiva(g,w,visitado);
        }
    }
}

void BEP(Grafo g, int origem) {
    int n; int *visitado;
    if (!g || !GBexisteIdVertice(g, origem)) { puts("Vertice de partida invalido."); return; }
    n = GInumeroVertices(g);
    visitado = (int *)calloc((size_t)n + 1, sizeof(*visitado));
    if (!visitado) { puts("Erro: memoria insuficiente."); return; }
    printf("Historico BEP: v%d", origem);
    BEPrecursiva(g, origem, visitado);
    puts(""); free(visitado);
}

void CMC(Grafo g, const float *pesos, int origem, int destino) {
    int n, i, v, e, escolhido, w, k = 0;
    float *dist; int *visitado, *antV, *antE, *caminhoV, *caminhoE;
    if (!g || !pesos || !GBexisteIdVertice(g,origem) || !GBexisteIdVertice(g,destino)) {
        puts("Vertice de partida/chegada invalido."); return;
    }
    n = GInumeroVertices(g);
    for (e = 1; e <= GInumeroArestas(g); ++e) {
        if (!isfinite(pesos[e]) || pesos[e] < 0.0f) {
            printf("CMC requer pesos finitos e nao negativos (aresta %d invalida).\n", e); return;
        }
    }
    dist = (float *)malloc(((size_t)n+1)*sizeof(*dist));
    visitado = (int *)calloc((size_t)n+1,sizeof(*visitado));
    antV = (int *)calloc((size_t)n+1,sizeof(*antV));
    antE = (int *)calloc((size_t)n+1,sizeof(*antE));
    caminhoV = (int *)malloc(((size_t)n+1)*sizeof(*caminhoV));
    caminhoE = (int *)malloc(((size_t)n+1)*sizeof(*caminhoE));
    if (!dist || !visitado || !antV || !antE || !caminhoV || !caminhoE) {
        free(dist); free(visitado); free(antV); free(antE); free(caminhoV); free(caminhoE);
        puts("Erro: memoria insuficiente."); return;
    }
    for (v=1; v<=n; ++v) dist[v]=FLT_MAX;
    dist[origem]=0.0f;
    for (i=0; i<n; ++i) {
        float menor=FLT_MAX; escolhido=0;
        for (v=1; v<=n; ++v) if (!visitado[v] && dist[v]<menor) { menor=dist[v]; escolhido=v; }
        if (!escolhido) break;
        if (escolhido==destino) break;
        visitado[escolhido]=1;
        for (e=GAprimaAresta(g,escolhido); e; e=GAproxAresta(g,escolhido,e)) {
            w=GVvizinho(g,e,escolhido);
            if (!w || visitado[w]) continue;
            if (dist[escolhido] + pesos[e] < dist[w]) {
                dist[w]=dist[escolhido]+pesos[e]; antV[w]=escolhido; antE[w]=e;
            }
        }
    }
    if (dist[destino]==FLT_MAX) {
        printf("Nao existe caminho entre v%d e v%d.\n",origem,destino);
    } else if (origem==destino) {
        printf("Caminho: v%d\nDistancia: 0\n",origem);
    } else {
        v=destino;
        while (v && k <= n) {
            caminhoV[k]=v;
            if (v==origem) break;
            caminhoE[k]=antE[v]; v=antV[v]; k++;
        }
        if (!v || caminhoV[k]!=origem) printf("Nao foi possivel reconstruir o caminho.\n");
        else {
            printf("Caminho: v%d",caminhoV[k]);
            for (i=k-1;i>=0;--i) printf(",a%d,v%d",caminhoE[i],caminhoV[i]);
            printf("\nDistancia: %.6g\n",dist[destino]);
        }
    }
    free(dist); free(visitado); free(antV); free(antE); free(caminhoV); free(caminhoE);
}

void CC(Grafo g) {
    int n, v, e, w, topo=0, componente=0;
    int *pilha, *visitado;
    if (!g) { puts("Grafo invalido."); return; }
    n=GInumeroVertices(g);
    pilha=(int *)malloc(((size_t)n+1)*sizeof(*pilha));
    visitado=(int *)calloc((size_t)n+1,sizeof(*visitado));
    if (!pilha || !visitado) { free(pilha); free(visitado); puts("Erro: memoria insuficiente."); return; }
    puts("Componentes conexos:");
    for (v=1;v<=n;++v) if (!visitado[v]) {
        componente++; topo=0; pilha[topo++]=v; visitado[v]=1;
        printf("{");
        { int primeiro=1;
          while (topo) {
              int x=pilha[--topo];
              printf("%s%d", primeiro ? "" : ",", x); primeiro=0;
              for (e=GAprimaAresta(g,x);e;e=GAproxAresta(g,x,e)) {
                  w=GVvizinho(g,e,x);
                  if (w && !visitado[w]) { visitado[w]=1; pilha[topo++]=w; }
              }
          }
        }
        puts("}");
    }
    if (!componente) puts("(grafo sem vertices)");
    free(pilha); free(visitado);
}
