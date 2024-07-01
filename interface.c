#include <ncurses.h>
#include "lista_encadeada.h"
#include "interface.h"

void lista_pedidos(WINDOW *tela, No *inicio)
{
    int largura = 20;
    int altura = 30;

    tela = newwin(altura, largura, 2, 2);
    box(tela, 0, 0);
    mvwprintw(tela, 1, (largura - strlen("PEDIDOS:")) / 2, "PEDIDOS");

    No *atual = inicio;
    int num_pedido = 1;
    int y_pos = 3;
    while (atual != NULL)
    {
        mvwprintw(tela, y_pos++, 2, "%d - %s", num_pedido, atual->pedido);
        num_pedido++;
        atual = atual->proximo;
    }

    wrefresh(tela);
}

void atualizar_pedidos(WINDOW *tela, No *inicio)
{
    werase(tela);
    box(tela, 0, 0);

    mvwprintw(tela, 1, (getmaxx(tela) - strlen("PEDIDOS:")) / 2, "PEDIDOS:");

    No *atual = inicio;
    int num_pedido = 1;
    int y_pos = 3;
    while (atual != NULL)
    {
        mvwprintw(tela, y_pos++, 2, "%d - %s", num_pedido, atual->pedido);
        num_pedido++;
        atual = atual->proximo;
    }

    wrefresh(tela);
}

void cria_bancada(WINDOW *tela, int num_bancada) {
    int largura = 12;
    int altura = 5;
    int X = (num_bancada - 1) * largura;
    int Y = 0;
    int alturaTela, larguraTela;
    getmaxyx(tela, alturaTela, larguraTela); 

    mvwvline(tela, Y, X, 0, altura);
    mvwvline(tela, Y, X + largura - 1, 0, altura);
    mvwhline(tela, Y, X, 0, largura);
    mvwhline(tela, Y + altura - 1, X, 0, largura);
    mvwaddch(tela, Y, X, ACS_ULCORNER);
    mvwaddch(tela, Y, X + largura - 1, ACS_URCORNER);
    mvwaddch(tela, Y + altura - 1, X, ACS_LLCORNER);
    mvwaddch(tela, Y + altura - 1, X + largura - 1, ACS_LRCORNER);

    char etiqueta[20];
    sprintf(etiqueta, "Bancada %d", num_bancada);
    mvwprintw(tela, Y + altura / 2, X + (largura - strlen(etiqueta)) / 2, "%s", etiqueta);

    wrefresh(tela);
}

void cria_cozinha(WINDOW *tela, int num_cozinha) {
    int largura = 12;
    int altura = 5;
    int X = (num_cozinha - 1) * largura;
    int alturaTela, larguraTela;
    getmaxyx(tela, alturaTela, larguraTela); 
    int Y = alturaTela - 15;

    mvwvline(tela, Y, X, 0, altura);
    mvwvline(tela, Y, X + largura - 1, 0, altura);
    mvwhline(tela, Y, X, 0, largura);
    mvwhline(tela, Y + altura - 1, X, 0, largura);
    mvwaddch(tela, Y, X, ACS_ULCORNER);
    mvwaddch(tela, Y, X + largura - 1, ACS_URCORNER);
    mvwaddch(tela, Y + altura - 1, X, ACS_LLCORNER);
    mvwaddch(tela, Y + altura - 1, X + largura - 1, ACS_LRCORNER);

    char etiqueta[20];
    sprintf(etiqueta, "Cozinha %d", num_cozinha);
    mvwprintw(tela, Y + altura / 2, X + (largura - strlen(etiqueta)) / 2, "%s", etiqueta);

    wrefresh(tela);
}

