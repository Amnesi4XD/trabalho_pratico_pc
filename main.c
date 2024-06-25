#include<stdio.h>
#include<pthread.h>
#include<ncurses.h>

void interface()
{
  attron(COLOR_PAIR(3));

  move(eixo_x, eixo_y);
  // Imprime a borda da caixa de texto
  for (int i = 0; i < largura; i++) {
    printw("-");
  }

  move(eixo_y + 1, eixo_x);
  printw("|");

  move(eixo_y + altura - 1, eixo_x);
  printw("|");

  for (int i = 0; i < largura; i++) {
    printw("-");
  }

  // Move o cursor para o centro da caieixo_xa de teeixo_xto
  move(eixo_y + altura / 2, eixo_x + largura / 2 - strlen(texto) / 2);

  // Imprime o textoo dentro da caieixo_xa de textoo
  printw("%s", texto);

  // Define a cor de fundo para preto
  attroff(COLOR_PAIR(3));
}

int main()
{
  int qtd_cozinheiros, qtd_bancadas;

  pthread_t interface_thread, entrada_thread;

  printf("Informe quantidade de cozinheiros:");
  scanf("%d", &qtd_cozinheiros);
  printf("Informe quantidade de bancadas:");
  scanf("%d", qtd_bancadas);

  pthread_create(&interface_thread, NULL, interface, NULL);
  for (int i = 0; i <qtd_cozinheiros; i++)
  {
    pthread_create
  }


}