/*
 * lista.c
 * Implementacao da lista declarada em lista.h: o corpo de cada operacao,
 * com o passo-a-passo dos ponteiros comentado.
 */
#include <stdio.h>
#include <stdlib.h>

#include "biblioteca/estruturas/lista.h"

void lista_iniciar(Lista *l) {
    l->inicio  = NULL;
    l->tamanho = 0;
}

/*
 * INSERIR NO INICIO -- o caso mais facil: nao precisa percorrer nada.
 *
 * Antes:   inicio -> [10] -> [20] -> NULL
 * Depois:  inicio -> [ 5] -> [10] -> [20] -> NULL
 */
void lista_inserir_inicio(Lista *l, int valor) {
    NoLista *novo;

    novo = (NoLista *) malloc(sizeof(NoLista));
    if (novo == NULL) {
        fprintf(stderr, "lista_inserir_inicio: sem memoria\n");
        return;
    }

    novo->valor   = valor;
    novo->proximo = l->inicio;   /* o novo aponta pro antigo primeiro */
    l->inicio     = novo;        /* e passa a ser o primeiro */

    l->tamanho++;
}

/*
 * INSERIR NO FIM -- aqui precisa CAMINHAR ate o ultimo no.
 * E' O(n) justamente porque a lista so conhece o inicio.
 *
 * Antes:   inicio -> [5] -> [10] -> NULL
 * Depois:  inicio -> [5] -> [10] -> [20] -> NULL
 */
void lista_inserir_fim(Lista *l, int valor) {
    NoLista *novo;
    NoLista *ultimo;

    novo = (NoLista *) malloc(sizeof(NoLista));
    if (novo == NULL) {
        fprintf(stderr, "lista_inserir_fim: sem memoria\n");
        return;
    }

    novo->valor   = valor;
    novo->proximo = NULL;   /* vai ficar no fim */

    if (l->inicio == NULL) {
        /* lista vazia: o novo no e' o proprio inicio */
        l->inicio = novo;
    } else {
        /* caminha enquanto houver um proximo; para no ULTIMO no */
        ultimo = l->inicio;
        while (ultimo->proximo != NULL) {
            ultimo = ultimo->proximo;
        }
        ultimo->proximo = novo;
    }

    l->tamanho++;
}

/*
 * INSERIR NUMA POSICAO -- para no no ANTERIOR a ela.
 * Inserir na pos 2 de [5, 10, 20]:
 *
 *   anterior = no da pos 1 = [10]
 *   novo->proximo = anterior->proximo   ([20])
 *   anterior->proximo = novo
 *
 * Resultado: [5, 10, novo, 20]
 */
void lista_inserir_posicao(Lista *l, int pos, int valor) {
    NoLista *novo;
    NoLista *anterior;
    int      i;

    /* pos == tamanho e' valido: significa "no fim" */
    if (pos < 0 || pos > l->tamanho) {
        fprintf(stderr, "lista_inserir_posicao: posicao %d invalida\n", pos);
        return;
    }

    novo = (NoLista *) malloc(sizeof(NoLista));
    if (novo == NULL) {
        fprintf(stderr, "lista_inserir_posicao: sem memoria\n");
        return;
    }
    novo->valor = valor;

    if (pos == 0) {
        /* CASO 1 - inserir no inicio: nao existe "anterior" para costurar,
           entao quem muda e' o proprio 'inicio' da lista. */
        novo->proximo = l->inicio;
        l->inicio     = novo;
    } else {
        /* CASO 2 - inserir depois de alguem: caminha pos-1 vezes para
           parar no no ANTERIOR a posicao desejada. */
        anterior = l->inicio;
        for (i = 0; i < pos - 1; i++) {
            anterior = anterior->proximo;
        }

        /* a ORDEM destas duas linhas importa: se invertidas, voce perde
           o resto da lista, porque anterior->proximo seria sobrescrito
           antes de alguem guardar aquele endereco */
        novo->proximo     = anterior->proximo;
        anterior->proximo = novo;
    }

    l->tamanho++;
}

/*
 * REMOVER -- tambem para no no ANTERIOR, para poder costurar a ligacao.
 * Remover a pos 1 de [5, 10, 20]:
 *
 *   anterior = [5],  removido = [10]
 *   anterior->proximo = removido->proximo   -> [5] passa a apontar pra [20]
 *   free(removido)
 */
int lista_remover_posicao(Lista *l, int pos) {
    NoLista *removido;
    NoLista *anterior;
    int      valor;
    int      i;

    if (pos < 0 || pos >= l->tamanho) {
        fprintf(stderr, "lista_remover_posicao: posicao %d invalida\n", pos);
        return -1;
    }

    if (pos == 0) {
        /* remover o primeiro: o inicio anda uma casa */
        removido  = l->inicio;
        l->inicio = removido->proximo;
    } else {
        anterior = l->inicio;
        for (i = 0; i < pos - 1; i++) {
            anterior = anterior->proximo;
        }
        removido          = anterior->proximo;
        anterior->proximo = removido->proximo;   /* pula o no removido */
    }

    valor = removido->valor;   /* copia antes do free */
    free(removido);

    l->tamanho--;
    return valor;
}

int lista_obter(Lista *l, int pos) {
    NoLista *atual;
    int      i;

    if (pos < 0 || pos >= l->tamanho) {
        fprintf(stderr, "lista_obter: posicao %d invalida\n", pos);
        return -1;
    }

    /* caminha pos vezes a partir do inicio */
    atual = l->inicio;
    for (i = 0; i < pos; i++) {
        atual = atual->proximo;
    }

    return atual->valor;
}

/* Busca linear: compara valor por valor, contando a posicao. */
int lista_buscar(Lista *l, int valor) {
    NoLista *atual;
    int      i;

    atual = l->inicio;
    i     = 0;
    while (atual != NULL) {
        if (atual->valor == valor) {
            return i;
        }
        atual = atual->proximo;
        i++;
    }

    return -1;   /* percorreu tudo e nao achou */
}

int lista_vazia(Lista *l) {
    return (l->inicio == NULL);
}

int lista_tamanho(Lista *l) {
    return l->tamanho;
}

void lista_imprimir(Lista *l) {
    NoLista *atual;

    printf("[ ");
    atual = l->inicio;
    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->proximo;
    }
    printf("]\n");
}

void lista_liberar(Lista *l) {
    NoLista *atual;
    NoLista *seguinte;

    atual = l->inicio;
    while (atual != NULL) {
        seguinte = atual->proximo;
        free(atual);
        atual = seguinte;
    }

    l->inicio  = NULL;
    l->tamanho = 0;
}
