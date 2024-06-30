#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include "lista_encadeada.h"

// Função para listar os pedidos
void lista_pedidos(No *inicio);

// Função para atualizar os pedidos
void atualizar_pedidos(No *inicio);

// Função para desenhar as bancadas
void cria_bancada(int num_bancada);

// Função para desenhar a cozinha
void cria_cozinha(int num_cozinha);

#endif // INTERFACE_H
