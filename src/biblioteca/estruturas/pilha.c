/*
 * Trabalho Pratico 1 - Estruturas de Dados 2
 * Aluno(s): PREENCHER NOME(S)
 * Matricula(s): PREENCHER MATRICULA(S)
 * Data: PREENCHER DATA
 */
/*
 * pilha.c
 * Implementacao da pilha declarada em pilha.h: o corpo de cada operacao,
 * com o passo-a-passo dos ponteiros comentado.
 */
#include <stdio.h>
#include <stdlib.h>

#include "biblioteca/estruturas/pilha.h"

void pilha_iniciar(Pilha *p) {
    p->topo    = NULL;   /* NULL = nao ha nenhum no ainda */
    p->tamanho = 0;
}

/*
 * EMPILHAR -- entra sempre pelo topo.
 *
 * Antes:   topo -> [3] -> [1] -> NULL
 * Depois:  topo -> [9] -> [3] -> [1] -> NULL
 */
void pilha_empilhar(Pilha *p, int valor) {
    NoPilha *novo;

    /* 1. cria o no novo na memoria dinamica */
    novo = (NoPilha *) malloc(sizeof(NoPilha));
    if (novo == NULL) {
        fprintf(stderr, "pilha_empilhar: sem memoria\n");
        return;
    }

    /* 2. guarda o valor */
    novo->valor = valor;

    /* 3. o novo no aponta para quem era o topo
          (se a pilha estava vazia, topo era NULL -- e da' certo igual) */
    novo->proximo = p->topo;

    /* 4. agora o topo e' o no novo */
    p->topo = novo;

    p->tamanho++;
}

/*
 * DESEMPILHAR -- sai sempre pelo topo.
 *
 * Antes:   topo -> [9] -> [3] -> [1] -> NULL
 * Depois:  topo -> [3] -> [1] -> NULL     (devolve 9)
 */
int pilha_desempilhar(Pilha *p) {
    NoPilha *removido;
    int      valor;

    if (p->topo == NULL) {
        fprintf(stderr, "pilha_desempilhar: pilha vazia\n");
        return -1;
    }

    /* 1. guarda o endereco do no que vai sair */
    removido = p->topo;

    /* 2. copia o valor ANTES de liberar, senao ele se perde */
    valor = removido->valor;

    /* 3. o topo passa a ser o no de baixo */
    p->topo = removido->proximo;

    /* 4. so agora devolve a memoria do no antigo */
    free(removido);

    p->tamanho--;
    return valor;
}

int pilha_topo(Pilha *p) {
    if (p->topo == NULL) {
        fprintf(stderr, "pilha_topo: pilha vazia\n");
        return -1;
    }
    return p->topo->valor;   /* le e pronto: nao mexe em ponteiro nenhum */
}

int pilha_vazia(Pilha *p) {
    return (p->topo == NULL);
}

int pilha_tamanho(Pilha *p) {
    return p->tamanho;
}

void pilha_imprimir(Pilha *p) {
    NoPilha *atual;

    printf("topo -> ");
    atual = p->topo;
    while (atual != NULL) {
        printf("[%d] ", atual->valor);
        atual = atual->proximo;   /* caminha um no para o lado */
    }
    printf("<- base\n");
}

/*
 * LIBERAR -- percorre devolvendo no por no.
 * O 'seguinte' e' obrigatorio: depois do free(atual), ler atual->proximo
 * seria acessar memoria que nao e' mais sua.
 */
void pilha_liberar(Pilha *p) {
    NoPilha *atual;
    NoPilha *seguinte;

    atual = p->topo;
    while (atual != NULL) {
        seguinte = atual->proximo;   /* guarda o caminho antes de destruir */
        free(atual);
        atual = seguinte;
    }

    p->topo    = NULL;
    p->tamanho = 0;
}
