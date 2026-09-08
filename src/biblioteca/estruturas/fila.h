/*
 * fila.h
 * Interface da fila (FIFO): a struct do no e os prototipos das operacoes.
 * Usada tambem pela busca em largura do grafo, por isso e' modulo separado.
 */
#ifndef FILA_H
#define FILA_H

typedef struct NoFila {
    int            valor;
    struct NoFila *proximo;
} NoFila;

/* A fila guarda as DUAS pontas:
   'inicio' porque e' de la que sai (desenfileirar),
   'fim'    porque e' la que entra (enfileirar).
   Sem o 'fim', cada insercao teria que percorrer a fila inteira -- O(n). */
typedef struct {
    NoFila *inicio;
    NoFila *fim;
    int     tamanho;
} Fila;

void fila_iniciar(Fila *f);
void fila_liberar(Fila *f);

void fila_enfileirar(Fila *f, int valor);   /* entra no fim */

/* Nao funcionam em fila vazia: chame fila_vazia antes.
   Se chamar do mesmo jeito, devolvem -1 e avisam no stderr. */
int  fila_desenfileirar(Fila *f);           /* sai do inicio */
int  fila_primeiro(Fila *f);                /* le o do inicio sem remover */

int  fila_vazia(Fila *f);
int  fila_tamanho(Fila *f);
void fila_imprimir(Fila *f);

#endif /* FILA_H */
