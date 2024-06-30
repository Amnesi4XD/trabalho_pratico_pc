#include <ncurses.h>
#include "lista_encadeada.h"
#include "interface.h"

void lista_pedidos(No *inicio)
{
    int X = 2;
    int Y = 2;
    int largura = 20;
    int altura = 15;

    mvhline(Y, X, 0, largura);
    mvhline(Y + altura - 1, X, 0, largura);
    mvvline(Y, X, 0, altura);
    mvvline(Y, X + largura - 1, 0, altura);
    mvaddch(Y, X, ACS_ULCORNER);
    mvaddch(Y, X + largura - 1, ACS_URCORNER);
    mvaddch(Y + altura - 1, X, ACS_LLCORNER);
    mvaddch(Y + altura - 1, X + largura - 1, ACS_LRCORNER);

    X = 3;
    Y = 3;
    mvprintw(Y, X,"Pedidos");
    Y++;
    No *atual = inicio;
    int num_pedido = 1;
    while (atual != NULL)
    {
        mvprintw(Y++, X, "%d - %s", num_pedido, atual->pedido);
        num_pedido++;
        atual = atual->proximo;
    }
}

void atualiza_pedidos(No *inicio)
{
    int X = 3;
    int Y = 3;
    No *atual = inicio;
    while (atual != NULL)
    {
        mvprintw(Y++, X, "%s", atual->pedido);
        atual = atual->proximo;
    }
}

// Função para desenhar as bancadas
void cria_bancada(int num_bancada) {
    int largura = 14;                         
    int altura = 5;                           
    int X = 35 + (num_bancada - 1) * largura; // Posição baseada no num_bancada
    int alturaTela, larguraTela;
    getmaxyx(stdscr, alturaTela, larguraTela); 
    int Y = 2; 

    // Desenhar o quadrado
    mvhline(Y, X, 0, largura);
    mvhline(Y + altura - 1, X, 0, largura);
    mvvline(Y, X, 0, altura);
    mvvline(Y, X + largura - 1, 0, altura);
    mvaddch(Y, X, ACS_ULCORNER);
    mvaddch(Y, X + largura - 1, ACS_URCORNER);
    mvaddch(Y + altura - 1, X, ACS_LLCORNER);
    mvaddch(Y + altura - 1, X + largura - 1, ACS_LRCORNER);

    // Imprimir a etiqueta dentro do quadrado
    char etiqueta[20];
    sprintf(etiqueta, "bancada %d", num_bancada);
    mvprintw(Y + altura / 2, X + (largura - strlen(etiqueta)) / 2, "%s", etiqueta);
}

// Função para desenhar a cozinha
void cria_cozinha(int num_cozinha) {
    int largura = 14;                          
    int altura = 5;
    int X = 35 + (num_cozinha - 1) * largura; // Posição baseada no num_cozinha
    int alturaTela, larguraTela;
    getmaxyx(stdscr, alturaTela, larguraTela); 
    int Y = alturaTela - 15;              

    // Desenhar o quadrado
    mvhline(Y, X, 0, largura);
    mvhline(Y + altura - 1, X, 0, largura);
    mvvline(Y, X, 0, altura);
    mvvline(Y, X + largura - 1, 0, altura);
    mvaddch(Y, X, ACS_ULCORNER);
    mvaddch(Y, X + largura - 1, ACS_URCORNER);
    mvaddch(Y + altura - 1, X, ACS_LLCORNER);
    mvaddch(Y + altura - 1, X + largura - 1, ACS_LRCORNER);

    // Imprimir a etiqueta dentro do quadrado
    char etiqueta[10];
    sprintf(etiqueta, "cozinha %d", num_cozinha);
    mvprintw(Y + altura / 2, X + (largura - strlen(etiqueta)) / 2, "%s", etiqueta);
}
