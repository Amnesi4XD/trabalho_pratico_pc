#include<stdio.h>

struct no
{
    char pedido[30];
    int tempo_bancada;
    int tempo_cozinha;
    struct no *proximo;
} No;


typedef struct no No;

// Função para inserir um novo item na lista
No *inserir(No *lista, char pedido[], int tempo_bancada, int tempo_cozinha) {
  No *novo = (No *)malloc(sizeof(No));
  strcpy(novo->pedido, pedido);
  novo->tempo_bancada = tempo_bancada;
  novo->tempo_cozinha = tempo_cozinha;
  novo->proximo = lista;
  return novo;
}

// Função para contar o número de itens na lista
int count(No *lista) {
  int contador = 0;
  while (lista != NULL) {
    contador++;
    lista = lista->proximo;
  }
  return contador;
}

// Função para remover um item da lista
No *remover(No *lista, char pedido[]) {
  No *anterior = NULL;
  No *atual = lista;

  while (atual != NULL && strcmp(atual->pedido, pedido) != 0) {
    anterior = atual;
    atual = atual->proximo;
  }

  if (atual != NULL) {
    if (anterior == NULL) {
      lista = atual->proximo;
    } else {
      anterior->proximo = atual->proximo;
    }
    free(atual);
  }

  return lista;
}
