#include "biblioteca/estruturas/grafo.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Adj {
    int aresta;
    struct Adj *prox;
} Adj;

typedef struct {
    Adj *incidentes; /* arestas incidentes: grafos nao dirigidos */
    Adj *entrada;    /* arestas que chegam */
    Adj *saida;      /* arestas que saem */
} Vertice;

typedef struct {
    int alfa;
    int omega;
} Aresta;

struct XXgrafoXX {
    int nvMax, naMax, nv, na;
    Vertice *vertices; /* indices validos de 1 a nvMax */
    Aresta *arestas;   /* indices validos de 1 a naMax */
};

static void libera_lista(Adj *p) {
    while (p) { Adj *q = p->prox; free(p); p = q; }
}
static int adiciona_adj(Adj **lista, int a) {
    Adj *n = (Adj *)malloc(sizeof(*n));
    if (!n) return 0;
    n->aresta = a; n->prox = *lista; *lista = n;
    return 1;
}
static int proxima_lista(Adj *p, int anterior) {
    int menor = 0;
    while (p) {
        if (p->aresta > anterior && (menor == 0 || p->aresta < menor))
            menor = p->aresta;
        p = p->prox;
    }
    return menor;
}

Grafo GGcriaGrafo(int nvMax, int naMax) {
    Grafo g;
    if (nvMax <= 0 || naMax <= 0) return NULL;
    g = (Grafo)calloc(1, sizeof(*g));
    if (!g) return NULL;
    g->vertices = (Vertice *)calloc((size_t)nvMax + 1, sizeof(Vertice));
    g->arestas = (Aresta *)calloc((size_t)naMax + 1, sizeof(Aresta));
    if (!g->vertices || !g->arestas) {
        free(g->vertices); free(g->arestas); free(g); return NULL;
    }
    g->nvMax = nvMax; g->naMax = naMax;
    return g;
}

void GGdestroiGrafo(Grafo g) {
    int i;
    if (!g) return;
    for (i = 1; i <= g->nvMax; ++i) {
        libera_lista(g->vertices[i].incidentes);
        libera_lista(g->vertices[i].entrada);
        libera_lista(g->vertices[i].saida);
    }
    free(g->vertices); free(g->arestas); free(g);
}

int GVcriaVertice(Grafo g) {
    if (!g || g->nv >= g->nvMax) return 0;
    return ++g->nv;
}

int GAcriaAresta(Grafo g, int v1, int v2) {
    int id;
    /* Nao altera o grafo se os extremos forem invalidos ou faltar capacidade. */
    if (!g || !GBexisteIdVertice(g, v1) || !GBexisteIdVertice(g, v2) ||
        g->na >= g->naMax) return 0;
    id = g->na + 1;
    /* Reserva todas as ligacoes antes de confirmar a aresta. */
    if (!adiciona_adj(&g->vertices[v1].saida, id)) return 0;
    if (!adiciona_adj(&g->vertices[v2].entrada, id)) {
        Adj *p=g->vertices[v1].saida;
        if (p && p->aresta==id) { g->vertices[v1].saida=p->prox; free(p); }
        return 0;
    }
    if (!adiciona_adj(&g->vertices[v1].incidentes, id)) {
        /* Em falha de memoria, remove referencias criadas e mantem consistencia. */
        Adj **pp=&g->vertices[v1].saida; while(*pp && (*pp)->aresta!=id) pp=&(*pp)->prox;
        if(*pp){Adj*t=*pp;*pp=t->prox;free(t);}
        pp=&g->vertices[v2].entrada; while(*pp && (*pp)->aresta!=id) pp=&(*pp)->prox;
        if(*pp){Adj*t=*pp;*pp=t->prox;free(t);}
        return 0;
    }
    if (v1 != v2 && !adiciona_adj(&g->vertices[v2].incidentes, id)) {
        Adj **pp=&g->vertices[v1].saida; while(*pp && (*pp)->aresta!=id) pp=&(*pp)->prox;
        if(*pp){Adj*t=*pp;*pp=t->prox;free(t);}
        pp=&g->vertices[v2].entrada; while(*pp && (*pp)->aresta!=id) pp=&(*pp)->prox;
        if(*pp){Adj*t=*pp;*pp=t->prox;free(t);}
        pp=&g->vertices[v1].incidentes; while(*pp && (*pp)->aresta!=id) pp=&(*pp)->prox;
        if(*pp){Adj*t=*pp;*pp=t->prox;free(t);}
        return 0;
    }
    g->arestas[id].alfa = v1;
    g->arestas[id].omega = v2;
    g->na = id;
    return id;
}

int GBexisteIdVertice(Grafo g, int v) { return g && v > 0 && v <= g->nv; }
int GBexisteIdAresta(Grafo g, int a) { return g && a > 0 && a <= g->na; }

int GApegaArestaDir(Grafo g, int v1, int v2) {
    int a;
    if (!GBexisteIdVertice(g,v1) || !GBexisteIdVertice(g,v2)) return 0;
    for (a=1; a<=g->na; ++a)
        if (g->arestas[a].alfa==v1 && g->arestas[a].omega==v2) return a;
    return 0;
}
int GApegaAresta(Grafo g, int v1, int v2) {
    int a=GApegaArestaDir(g,v1,v2);
    return a ? a : GApegaArestaDir(g,v2,v1);
}
int GBexisteArestaDir(Grafo g, int v1, int v2) { return GApegaArestaDir(g,v1,v2)!=0; }
int GBexisteAresta(Grafo g, int v1, int v2) { return GApegaAresta(g,v1,v2)!=0; }

int GVprimeiroVertice(Grafo g) { return (g && g->nv) ? 1 : 0; }
int GVproximoVertice(Grafo g, int v1) {
    if (!g) return 0;
    return (v1 >= 1 && v1 < g->nv) ? v1+1 : 0;
}
int GAprimeiraAresta(Grafo g) { return (g && g->na) ? 1 : 0; }
int GAproximaAresta(Grafo g, int a1) {
    if (!g || a1 < 1 || a1 >= g->na) return 0;
    return a1+1;
}
int GInumeroVertices(Grafo g) { return g ? g->nv : 0; }
int GInumeroVerticesMax(Grafo g) { return g ? g->nvMax : 0; }
int GInumeroArestas(Grafo g) { return g ? g->na : 0; }
int GInumeroArestasMax(Grafo g) { return g ? g->naMax : 0; }

Grafo GGcarregaGrafo(const char *arquivo) {
    FILE *f;
    int nv, na, i, u, v;
    Grafo g;
    if (!arquivo || !(f=fopen(arquivo,"r"))) return NULL;
    if (fscanf(f,"%d%d",&nv,&na)!=2 || nv<=0 || na<=0) { fclose(f); return NULL; }
    g=GGcriaGrafo(nv,na);
    if (!g) { fclose(f); return NULL; }
    for (i=0;i<nv;i++) if (!GVcriaVertice(g)) { GGdestroiGrafo(g); fclose(f); return NULL; }
    for (i=0;i<na;i++) {
        if (fscanf(f,"%d%d",&u,&v)!=2 || !GAcriaAresta(g,u,v)) {
            GGdestroiGrafo(g); fclose(f); return NULL;
        }
    }
    fclose(f);
    return g;
}
int GBsalvaGrafo(Grafo g, const char *arquivo) {
    FILE *f; int a;
    if (!g || !arquivo || !(f=fopen(arquivo,"w"))) return 0;
    fprintf(f,"%d %d\n",g->nv,g->na);
    for (a=1;a<=g->na;a++) fprintf(f,"%d %d\n",g->arestas[a].alfa,g->arestas[a].omega);
    if (fclose(f)!=0) return 0;
    return 1;
}

int GIpegaGrau(Grafo g, int v) {
    int a, grau=0;
    if (!GBexisteIdVertice(g,v)) return 0;
    for (a=1;a<=g->na;a++)
        if (g->arestas[a].alfa==v || g->arestas[a].omega==v)
            grau += (g->arestas[a].alfa==v && g->arestas[a].omega==v) ? 2 : 1;
    return grau;
}
int GAprimaAresta(Grafo g, int v) {
    if (!GBexisteIdVertice(g,v)) return 0;
    return proxima_lista(g->vertices[v].incidentes,0);
}
int GAproxAresta(Grafo g, int v, int a1) {
    if (!GBexisteIdVertice(g,v)) return 0;
    return proxima_lista(g->vertices[v].incidentes,a1);
}
int GAprimaEntrada(Grafo g, int v) {
    if (!GBexisteIdVertice(g,v)) return 0;
    return proxima_lista(g->vertices[v].entrada,0);
}
int GAproxEntrada(Grafo g, int v, int a1) {
    if (!GBexisteIdVertice(g,v)) return 0;
    return proxima_lista(g->vertices[v].entrada,a1);
}
int GAprimaSaida(Grafo g, int v) {
    if (!GBexisteIdVertice(g,v)) return 0;
    return proxima_lista(g->vertices[v].saida,0);
}
int GAproxSaida(Grafo g, int v, int a1) {
    if (!GBexisteIdVertice(g,v)) return 0;
    return proxima_lista(g->vertices[v].saida,a1);
}
int GBarestaLaco(Grafo g, int a) {
    return GBexisteIdAresta(g,a) && g->arestas[a].alfa==g->arestas[a].omega;
}
int GValfa(Grafo g, int a) { return GBexisteIdAresta(g,a) ? g->arestas[a].alfa : 0; }
int GVomega(Grafo g, int a) { return GBexisteIdAresta(g,a) ? g->arestas[a].omega : 0; }
int GVvizinho(Grafo g, int a, int v1) {
    if (!GBexisteIdAresta(g,a) || !GBexisteIdVertice(g,v1)) return 0;
    if (g->arestas[a].alfa==v1) return g->arestas[a].omega;
    if (g->arestas[a].omega==v1) return g->arestas[a].alfa;
    return 0;
}
