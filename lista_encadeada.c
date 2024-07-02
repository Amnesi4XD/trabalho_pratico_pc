#include <stdio.h>
#include "lista_encadeada.h"

No *inserir(No *lista, char pedido[], int tempo_bancada, int tempo_cozinha, int pontos) {
    No *novo = (No *)malloc(sizeof(No));
    novo->proximo = NULL;
    strcpy(novo->pedido, pedido);
    novo->tempo_bancada = tempo_bancada;
    novo->tempo_cozinha = tempo_cozinha;
    novo->pontos = pontos;

    if (lista == NULL) {
        return novo;
    }

    No *atual = lista;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }

    atual->proximo = novo;

    return lista;
}



int count(No *lista) {
  int contador = 0;
  while (lista != NULL) {
    contador++;
    lista = lista->proximo;
  }
  return contador;
}

No *remover(No *lista, int indice_pedido) {
    No *anterior = NULL;
    No *atual = lista;
    int contador = 0;

    while (atual != NULL && contador < indice_pedido) {
        anterior = atual;
        atual = atual->proximo;
        contador++;
    }

    if (atual == NULL) {
        return lista;
    }

    if (anterior == NULL) {
        lista = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    return lista;
}

