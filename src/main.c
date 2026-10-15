/*
 * Trabalho Pratico - Algoritmos em Grafos (ED2)
 * Aluno(s): PREENCHER NOME(S)
 * Matricula(s): PREENCHER MATRICULA(S)
 * Data: PREENCHER DATA
 */
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca/estruturas/grafo.h"
#include "biblioteca/estruturas/algoritmos.h"

#define TAM_LINHA 512

typedef struct {
    Grafo grafo;
    float *pesos; /* pesos[1..numero de arestas] */
} Entrada;

static int leInteiro(const char *mensagem, int *valor) {
    char linha[TAM_LINHA], *fim;
    long x;
    printf("%s", mensagem);
    if (!fgets(linha,sizeof(linha),stdin)) return 0;
    errno=0; x=strtol(linha,&fim,10);
    while (isspace((unsigned char)*fim)) fim++;
    if (fim==linha || *fim!='\0' || errno || x<0 || x>2147483647L) return 0;
    *valor=(int)x; return 1;
}

static int linhaUtil(FILE *f, char *linha, size_t tamanho) {
    while (fgets(linha,(int)tamanho,f)) {
        char *p=linha;
        while (isspace((unsigned char)*p)) p++;
        if (*p=='\0' || *p=='\n' || *p=='#') continue;
        return 1;
    }
    return 0;
}

static void liberaEntrada(Entrada *ent) {
    if (!ent) return;
    GGdestroiGrafo(ent->grafo);
    free(ent->pesos);
    ent->grafo=NULL; ent->pesos=NULL;
}

/* Formato: primeira linha n m; depois m linhas "origem destino [peso]".
   Peso e opcional (default 1). Linhas vazias e comentarios iniciados por # sao ignorados. */
static int carregaArquivo(const char *nome, Entrada *ent) {
    FILE *f=fopen(nome,"r");
    char linha[TAM_LINHA], extra;
    int n,m,i,u,v,campos,id;
    float peso;
    Grafo g=NULL; float *pesos=NULL;
    if (!f) { perror("Nao foi possivel abrir o arquivo"); return 0; }
    if (!linhaUtil(f,linha,sizeof(linha)) || sscanf(linha,"%d %d %c",&n,&m,&extra)!=2 || n<=0 || m<0) {
        fprintf(stderr,"Cabecalho invalido. Use: numero_de_vertices numero_de_arestas\n"); fclose(f); return 0;
    }
    g=GGcriaGrafo(n,m>0?m:1);
    pesos=(float*)calloc((size_t)m+1,sizeof(*pesos));
    if (!g || !pesos) { fprintf(stderr,"Memoria insuficiente.\n"); GGdestroiGrafo(g); free(pesos); fclose(f); return 0; }
    for (i=0;i<n;i++) if (!GVcriaVertice(g)) {
        fprintf(stderr,"Falha ao criar vertices.\n"); GGdestroiGrafo(g); free(pesos); fclose(f); return 0;
    }
    for (i=0;i<m;i++) {
        if (!linhaUtil(f,linha,sizeof(linha))) {
            fprintf(stderr,"Arquivo terminou antes de ler as %d arestas.\n",m);
            GGdestroiGrafo(g); free(pesos); fclose(f); return 0;
        }
        peso=1.0f;
        campos=sscanf(linha,"%d %d %f %c",&u,&v,&peso,&extra);
        if ((campos!=2 && campos!=3) || !isfinite(peso) || u<1 || u>n || v<1 || v>n) {
            fprintf(stderr,"Linha de aresta invalida: %s",linha);
            GGdestroiGrafo(g); free(pesos); fclose(f); return 0;
        }
        id=GAcriaAresta(g,u,v);
        if (!id) { fprintf(stderr,"Nao foi possivel criar a aresta %d.\n",i+1); GGdestroiGrafo(g); free(pesos); fclose(f); return 0; }
        pesos[id]=peso;
    }
    /* Rejeita linhas de dados excedentes, ignorando linhas vazias/comentarios. */
    while (fgets(linha,sizeof(linha),f)) {
        char *p=linha; while (isspace((unsigned char)*p)) p++;
        if (*p!='\0' && *p!='\n' && *p!='#') {
            fprintf(stderr,"Ha dados extras depois das %d arestas.\n",m);
            GGdestroiGrafo(g); free(pesos); fclose(f); return 0;
        }
    }
    fclose(f); ent->grafo=g; ent->pesos=pesos; return 1;
}

static int criaManual(Entrada *ent) {
    int n,m,i,u,v,id; char linha[TAM_LINHA], extra; float peso; Grafo g; float *pesos;
    if (!leInteiro("Numero de vertices: ",&n) || n<=0 ||
        !leInteiro("Numero de arestas: ",&m)) {
        puts("Entrada invalida."); return 0;
    }
    g=GGcriaGrafo(n,m>0?m:1); pesos=(float*)calloc((size_t)m+1,sizeof(*pesos));
    if (!g || !pesos) { GGdestroiGrafo(g); free(pesos); puts("Memoria insuficiente."); return 0; }
    for (i=0;i<n;i++) if (!GVcriaVertice(g)) { GGdestroiGrafo(g); free(pesos); puts("Falha ao criar vertices."); return 0; }
    printf("Informe cada aresta como: origem destino peso\n");
    for (i=0;i<m;i++) {
        int campos;
        printf("Aresta %d: ",i+1);
        if (!fgets(linha,sizeof(linha),stdin)) { GGdestroiGrafo(g); free(pesos); return 0; }
        peso=1.0f; campos=sscanf(linha,"%d %d %f %c",&u,&v,&peso,&extra);
        if ((campos!=2 && campos!=3) || !isfinite(peso) || u<1 || u>n || v<1 || v>n) {
            GGdestroiGrafo(g); free(pesos); puts("Aresta invalida."); return 0;
        }
        id=GAcriaAresta(g,u,v);
        if (!id) { GGdestroiGrafo(g); free(pesos); puts("Nao foi possivel criar aresta."); return 0; }
        pesos[id]=peso;
    }
    ent->grafo=g; ent->pesos=pesos; return 1;
}

static void menuAlgoritmos(Entrada *ent) {
    int opcao, origem, destino;
    Grafo g=ent->grafo;
    printf("\nGrafo carregado: %d vertices, %d arestas.\n",GInumeroVertices(g),GInumeroArestas(g));
    for (;;) {
        puts("\n========== ALGORITMOS EM GRAFOS ==========");
        puts("1 - Arvore Geradora Minima (AGM / Kruskal)");
        puts("2 - Busca em Largura (BEL)");
        puts("3 - Busca em Profundidade (BEP)");
        puts("4 - Caminho mais curto (CMC / Dijkstra)");
        puts("5 - Componentes conexos (CC)");
        puts("0 - Sair");
        if (!leInteiro("Escolha: ",&opcao)) { puts("Opcao invalida."); continue; }
        switch (opcao) {
            case 1: AGM(g,ent->pesos); break;
            case 2:
                if (leInteiro("Vertice de partida: ",&origem)) BEL(g,origem);
                else puts("Entrada invalida.");
                break;
            case 3:
                if (leInteiro("Vertice de partida: ",&origem)) BEP(g,origem);
                else puts("Entrada invalida.");
                break;
            case 4:
                if (leInteiro("Vertice de partida: ",&origem) && leInteiro("Vertice de chegada: ",&destino)) CMC(g,ent->pesos,origem,destino);
                else puts("Entrada invalida.");
                break;
            case 5: CC(g); break;
            case 0: return;
            default: puts("Opcao inexistente."); break;
        }
    }
}

int main(void) {
    Entrada ent={NULL,NULL};
    int opcao; char nome[TAM_LINHA];
    puts("Trabalho de ED2 - Algoritmos em Grafos");
    puts("1 - Carregar grafo de arquivo");
    puts("2 - Digitar grafo manualmente");
    if (!leInteiro("Escolha: ",&opcao)) { puts("Entrada invalida."); return 1; }
    if (opcao==1) {
        printf("Caminho do arquivo: ");
        if (!fgets(nome,sizeof(nome),stdin)) return 1;
        nome[strcspn(nome,"\r\n")]='\0';
        if (!carregaArquivo(nome,&ent)) return 1;
    } else if (opcao==2) {
        if (!criaManual(&ent)) return 1;
    } else { puts("Opcao inexistente."); return 1; }
    menuAlgoritmos(&ent);
    liberaEntrada(&ent);
    return 0;
}
