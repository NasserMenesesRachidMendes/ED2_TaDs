/*
 * lista.h
 * Interface da lista encadeada: a struct do no e as operacoes por posicao.
 * Base para as estruturas que precisam de tamanho dinamico.
 */
#ifndef LISTA_H
#define LISTA_H

typedef struct NoLista {
    int             valor;
    struct NoLista *proximo;
} NoLista;

/* A lista guarda apenas o primeiro no. Diferente da fila, aqui a insercao
   pode acontecer em qualquer posicao, entao guardar o fim ajudaria menos. */
typedef struct {
    NoLista *inicio;
    int      tamanho;
} Lista;

void lista_iniciar(Lista *l);
void lista_liberar(Lista *l);

void lista_inserir_inicio(Lista *l, int valor);   /* posicao 0     -- O(1) */
void lista_inserir_fim(Lista *l, int valor);      /* apos o ultimo -- O(n) */
void lista_inserir_posicao(Lista *l, int pos, int valor);  /* 0 ate tamanho */

/* Devolvem -1 e avisam no stderr se a posicao nao existir. */
int  lista_remover_posicao(Lista *l, int pos);    /* remove e devolve o valor */
int  lista_obter(Lista *l, int pos);              /* le sem remover */

int  lista_buscar(Lista *l, int valor);   /* devolve a posicao, ou -1 */
int  lista_vazia(Lista *l);
int  lista_tamanho(Lista *l);
void lista_imprimir(Lista *l);

#endif /* LISTA_H */
