/*
 * main.c
 * Ponto de entrada do programa. Apenas orquestra: chama as operacoes dos
 * modulos de biblioteca/. Nao implementa estrutura alguma.
 */
#include <stdio.h>

#include "biblioteca/estruturas/pilha.h"
#include "biblioteca/estruturas/fila.h"
#include "biblioteca/estruturas/lista.h"

int main(void) {
    /* As estruturas em si sao variaveis normais; o que e' dinamico sao
       os NOS, alocados com malloc dentro de cada insercao. */
    Pilha p;
    Fila  f;
    Lista l;
    int   i;

    pilha_iniciar(&p);
    fila_iniciar(&f);
    lista_iniciar(&l);

    printf("=== PILHA (LIFO: sai o ultimo que entrou) ===\n");
    for (i = 1; i <= 4; i++) {
        pilha_empilhar(&p, i);
        printf("empilhar(%d)   ", i);
        pilha_imprimir(&p);
    }
    printf("topo()         -> %d\n", pilha_topo(&p));
    printf("desempilhar()  -> %d   ", pilha_desempilhar(&p));
    pilha_imprimir(&p);
    printf("tamanho()      -> %d\n", pilha_tamanho(&p));

    printf("\n=== FILA (FIFO: sai o primeiro que entrou) ===\n");
    for (i = 1; i <= 4; i++) {
        fila_enfileirar(&f, i);
        printf("enfileirar(%d)   ", i);
        fila_imprimir(&f);
    }
    printf("primeiro()        -> %d\n", fila_primeiro(&f));
    printf("desenfileirar()   -> %d   ", fila_desenfileirar(&f));
    fila_imprimir(&f);
    printf("tamanho()         -> %d\n", fila_tamanho(&f));

    printf("\n=== LISTA ENCADEADA ===\n");
    lista_inserir_fim(&l, 10);
    printf("inserir_fim(10)         ");    lista_imprimir(&l);
    lista_inserir_fim(&l, 30);
    printf("inserir_fim(30)         ");    lista_imprimir(&l);
    lista_inserir_inicio(&l, 5);
    printf("inserir_inicio(5)       ");    lista_imprimir(&l);
    lista_inserir_posicao(&l, 2, 20);
    printf("inserir_posicao(2, 20)  ");    lista_imprimir(&l);

    printf("obter(0)             -> %d\n", lista_obter(&l, 0));
    printf("buscar(20)           -> %d\n", lista_buscar(&l, 20));
    printf("buscar(99)           -> %d  (nao existe)\n", lista_buscar(&l, 99));
    printf("remover_posicao(0)   -> %d   ", lista_remover_posicao(&l, 0));
    lista_imprimir(&l);
    printf("tamanho()            -> %d\n", lista_tamanho(&l));

    printf("\n=== CASOS DE ERRO (avisos no stderr) ===\n");
    fflush(stdout);           /* garante a ordem das mensagens na tela */
    while (!pilha_vazia(&p)) {
        pilha_desempilhar(&p);
    }
    pilha_desempilhar(&p);    /* pilha vazia */
    lista_obter(&l, 99);      /* posicao inexistente */

    /* Devolve ao sistema toda a memoria alocada pelos nos. */
    pilha_liberar(&p);
    fila_liberar(&f);
    lista_liberar(&l);

    return 0;
}
