#include "lista_encadeada.h"
#include "interface.h"

void lista_pedidos(WINDOW *tela)
{
    int largura = 20;
    int altura = 30;

    tela = newwin(altura, largura, 2, 2);
    box(tela, 0, 0);
    mvwprintw(tela, 1, (largura - strlen("PEDIDOS:")) / 2, "PEDIDOS:");
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

void cria_bancada(int num_bancada)
{
    int largura = 15;
    int altura = 5;
    int X = 35 + (num_bancada - 1) * largura;
    int Y = 2;

    mvhline(Y, X, 0, largura);
    mvhline(Y + altura - 1, X, 0, largura);
    mvvline(Y, X, 0, altura);
    mvvline(Y, X + largura - 1, 0, altura);
    mvaddch(Y, X, ACS_ULCORNER);
    mvaddch(Y, X + largura - 1, ACS_URCORNER);
    mvaddch(Y + altura - 1, X, ACS_LLCORNER);
    mvaddch(Y + altura - 1, X + largura - 1, ACS_LRCORNER);

    char etiqueta[20];
    sprintf(etiqueta, "bancada %d", num_bancada);
    mvprintw(Y + altura / 2 - 1, X + (largura - strlen(etiqueta)) / 2, "%s", etiqueta);

    refresh();
}

void cria_cozinha(int num_cozinha)
{
    int largura = 15;
    int altura = 5;
    int X = 35 + (num_cozinha - 1) * largura;
    int alturaTela, larguraTela;
    getmaxyx(stdscr, alturaTela, larguraTela);
    int Y = alturaTela - 13;

    mvhline(Y, X, 0, largura);
    mvhline(Y + altura - 1, X, 0, largura);
    mvvline(Y, X, 0, altura);
    mvvline(Y, X + largura - 1, 0, altura);
    mvaddch(Y, X, ACS_ULCORNER);
    mvaddch(Y, X + largura - 1, ACS_URCORNER);
    mvaddch(Y + altura - 1, X, ACS_LLCORNER);
    mvaddch(Y + altura - 1, X + largura - 1, ACS_LRCORNER);

    char etiqueta[20];
    sprintf(etiqueta, "cozinha %d", num_cozinha);
    mvprintw(Y + altura / 2 - 1, X + (largura - strlen(etiqueta)) / 2, "%s", etiqueta);

    refresh();
}

void cria_box_cozinheiros(int num_cozinheiros)
{
    int largura = 15;
    int altura = 5;
    int alturaTela, larguraTela;
    getmaxyx(stdscr, alturaTela, larguraTela);

    int X = 48;
    int Y = 15;

    mvhline(Y, X, 0, largura);
    mvhline(Y + altura - 1, X, 0, largura);
    mvvline(Y, X, 0, altura);
    mvvline(Y, X + largura - 1, 0, altura);
    mvaddch(Y, X, ACS_ULCORNER);
    mvaddch(Y, X + largura - 1, ACS_URCORNER);
    mvaddch(Y + altura - 1, X, ACS_LLCORNER);
    mvaddch(Y + altura - 1, X + largura - 1, ACS_LRCORNER);

    for (int i = 1; i <= num_cozinheiros; i++) {
        char etiqueta[20];
        sprintf(etiqueta, "Cozinheiro %d", i);
        mvprintw(Y + 1 , X + 1, "%s", etiqueta);
        Y++;
    }
    refresh();
}
