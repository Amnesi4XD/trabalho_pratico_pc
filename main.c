#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "interface.h"
#include "lista_encadeada.h"
#include<ncurses.h>

int tempo_jogo = 60;


struct Configuracao
{
  int qtd_cozinheiros;
  int qtd_bancadas;
  int qtd_cozinhas;
  No *pedidos;
} configuracao;

void *temporizador()
{
  while(tempo_jogo > 0)
  {
  sleep(1);
  tempo_jogo = tempo_jogo - 1;
  }
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

  pthread_t tela;
  initscr();
  raw();
  noecho();	
  pthread_create(&tela, NULL, interface, (void *)&configuracao);
  pthread_join(tela, NULL); // Aguarda a finalização da thread 'tela'
  while(true)
  {
    if(tempo_jogo == 0)
    {
      endwin();
    }
  }
  return 0;
}