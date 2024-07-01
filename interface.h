#ifndef INTERFACE_H
#define INTERFACE_H

#include <ncurses.h>
#include "lista_encadeada.h"

void lista_pedidos(WINDOW *tela, No *inicio);
void atualizar_pedidos(WINDOW *tela, No *inicio);
void cria_bancada(WINDOW *tela, int num_bancada);
void cria_cozinha(WINDOW *tela, int num_cozinha);

#endif 
