/*
 * Trabalho Pratico 1 - Estruturas de Dados 2
 * Aluno(s): PREENCHER NOME(S)
 * Matricula(s): PREENCHER MATRICULA(S)
 * Data: PREENCHER DATA
 */
/*
 * fila.c
 * Implementacao da fila declarada em fila.h: o corpo de cada operacao,
 * com o passo-a-passo dos ponteiros comentado.
 */
#include <stdio.h>
#include <stdlib.h>

#include "biblioteca/estruturas/fila.h"

void fila_iniciar(Fila *f) {
    f->inicio  = NULL;
    f->fim     = NULL;
    f->tamanho = 0;
}

/*
 * ENFILEIRAR -- entra pelo FIM.
 *
 * Antes:   inicio -> [1] -> [2] -> NULL <- fim
 * Depois:  inicio -> [1] -> [2] -> [7] -> NULL <- fim
 */
void fila_enfileirar(Fila *f, int valor) {
    NoFila *novo;

    novo = (NoFila *) malloc(sizeof(NoFila));
    if (novo == NULL) {
        fprintf(stderr, "fila_enfileirar: sem memoria\n");
        return;
    }

    novo->valor   = valor;
    novo->proximo = NULL;   /* entra no fim, entao nao ha ninguem depois dele */

    /* Dois casos diferentes: */
    if (f->fim == NULL) {
        /* CASO 1 - fila vazia: o no novo e' o inicio E o fim */
        f->inicio = novo;
    } else {
        /* CASO 2 - ja' tem gente: quem era o ultimo passa a apontar pro novo */
        f->fim->proximo = novo;
    }

    /* nos dois casos, o novo passa a ser o fim */
    f->fim = novo;

    f->tamanho++;
}

/*
 * DESENFILEIRAR -- sai pelo INICIO.
 *
 * Antes:   inicio -> [1] -> [2] -> [7] -> NULL <- fim
 * Depois:  inicio -> [2] -> [7] -> NULL <- fim    (devolve 1)
 */
int fila_desenfileirar(Fila *f) {
    NoFila *removido;
    int     valor;

    if (f->inicio == NULL) {
        fprintf(stderr, "fila_desenfileirar: fila vazia\n");
        return -1;
    }

    removido = f->inicio;
    valor    = removido->valor;

    /* o inicio anda uma casa para frente */
    f->inicio = removido->proximo;

    /* ATENCAO: se acabou de sair o ULTIMO no, o 'fim' ainda esta apontando
       para o no que vamos liberar. Ele precisa virar NULL tambem, senao fica
       apontando para memoria invalida e a proxima insercao quebra. */
    if (f->inicio == NULL) {
        f->fim = NULL;
    }

    free(removido);

    f->tamanho--;
    return valor;
}

int fila_primeiro(Fila *f) {
    if (f->inicio == NULL) {
        fprintf(stderr, "fila_primeiro: fila vazia\n");
        return -1;
    }
    return f->inicio->valor;
}

int fila_vazia(Fila *f) {
    return (f->inicio == NULL);
}

int fila_tamanho(Fila *f) {
    return f->tamanho;
}

void fila_imprimir(Fila *f) {
    NoFila *atual;

    printf("inicio -> ");
    atual = f->inicio;
    while (atual != NULL) {
        printf("[%d] ", atual->valor);
        atual = atual->proximo;
    }
    printf("<- fim\n");
}

void fila_liberar(Fila *f) {
    NoFila *atual;
    NoFila *seguinte;

    atual = f->inicio;
    while (atual != NULL) {
        seguinte = atual->proximo;
        free(atual);
        atual = seguinte;
    }

    f->inicio  = NULL;
    f->fim     = NULL;
    f->tamanho = 0;
}
