/*
 * pilha.h
 * Interface da pilha (LIFO): a struct do no e os prototipos das operacoes.
 * Unico arquivo que quem usa a pilha precisa incluir.
 */
#ifndef PILHA_H
#define PILHA_H

/* Um no da pilha. O 'proximo' guarda o ENDERECO do no de baixo --
   e' isso que amarra os nos numa corrente. O ultimo aponta para NULL. */
typedef struct NoPilha {
    int             valor;
    struct NoPilha *proximo;
} NoPilha;

/* A pilha so precisa saber quem esta no topo.
   'tamanho' e' conveniencia: da para contar percorrendo, mas custa O(n). */
typedef struct {
    NoPilha *topo;
    int      tamanho;
} Pilha;

void pilha_iniciar(Pilha *p);    /* deixa a pilha vazia -- chame antes de usar */
void pilha_liberar(Pilha *p);    /* devolve a memoria de todos os nos */

void pilha_empilhar(Pilha *p, int valor);

/* Nao funcionam em pilha vazia: chame pilha_vazia antes.
   Se chamar do mesmo jeito, devolvem -1 e avisam no stderr. */
int  pilha_desempilhar(Pilha *p);   /* remove o do topo e devolve o valor */
int  pilha_topo(Pilha *p);          /* le o do topo sem remover */

int  pilha_vazia(Pilha *p);
int  pilha_tamanho(Pilha *p);
void pilha_imprimir(Pilha *p);

#endif /* PILHA_H */
