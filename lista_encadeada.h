#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    char pedido[30];
    int tempo_bancada;
    int tempo_cozinha;
    int pontos;
    struct no *proximo;
} No;

typedef struct bancada{
    char entra_pedido[64];
    int status;
}Bancada;

// Função para inserir um novo item na lista
No *inserir(No *lista, char pedido[], int tempo_bancada, int tempo_cozinha, int pontos);

// Função para contar o número de itens na lista
int count(No *lista);

// Função para remover um item da lista
No *remover(No *lista, int indice_pedido);

#endif //
