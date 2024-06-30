#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "interface.h"
#include "lista_encadeada.h"
#include<ncurses.h>

int tempo_jogo = 60;

pthread_mutex_t mut_temp;

struct Configuracao
{
  int qtd_cozinheiros;
  int qtd_bancadas;
  int qtd_cozinhas;
  No *pedidos;
} configuracao;

void *temporizador(void *arg) {
    WINDOW *temporizador = (WINDOW *)arg;
    int altura_tela, largura_tela;
    getmaxyx(stdscr, altura_tela, largura_tela);
    int altura_temp = 3; 
    int largura_temp = 15; 
    int starty = 1;  
    int startx = largura_tela - largura_temp - 1;

    temporizador = newwin(altura_temp, largura_temp, starty, startx);
    box(temporizador, 0, 0);

    while (tempo_jogo > 0) {
        sleep(1);
        pthread_mutex_lock(&mut_temp);
        tempo_jogo--;
        pthread_mutex_unlock(&mut_temp);

        werase(temporizador);
        box(temporizador, 0, 0);
        mvwprintw(temporizador, 1, 2, "TEMPO: %d", tempo_jogo);
        wrefresh(temporizador);
    }
    
    delwin(temporizador);
    return NULL;
}
void *interface(void *configuracao)
{
  initscr();	
  struct Configuracao *config = (struct Configuracao *)configuracao;
  lista_pedidos(config->pedidos);
  for (int i = 1; i <= config->qtd_bancadas; i++)
  {
    cria_bancada(i);
  }
  for (int i = 1; i <= config->qtd_cozinhas; i++)
  {
    cria_cozinha(i);
  }
  refresh();
  return NULL;
}

int main()
{
  printf("Informe a quantidade de cozinheiros: ");
  scanf("%d", &configuracao.qtd_cozinheiros);

  printf("\nInforme a quantidade de bancadas: ");
  scanf("%d", &configuracao.qtd_bancadas);

  printf("\nInforme a quantidade de cozinhas: ");
  scanf("%d", &configuracao.qtd_cozinhas);
  No *lista = NULL;
  int ped;
  for (int i = 0; i < 10; i++)
  {
    ped = (rand() % 5) + 1;
    switch (ped)
    {
    case 1:
      lista = inserir(lista, "Hamburguer", 5, 10);
      break;
    case 2:
      lista = inserir(lista, "Pizza", 4, 8);
      break;
    case 3:
      lista = inserir(lista, "salada", 4, 1);
      break;
    case 4:
      lista = inserir(lista, "suco", 2, 2);
      break;
    case 5:
      lista = inserir(lista, "macarrão", 6, 8);
      break;
    default:
      break;
    }
  }
  configuracao.pedidos = lista;

  pthread_t thread_tela, thread_temporizador;
  initscr();
  raw();
  noecho();	
  pthread_create(&thread_tela, NULL, interface, (void *)&configuracao);
  pthread_create(&thread_temporizador, NULL, temporizador, NULL);
  pthread_join(thread_tela, NULL);
  pthread_join(thread_temporizador, NULL);
  while(true)
  {
    if(tempo_jogo == 0)
    {
      endwin();
    }
  }
  return 0;
}