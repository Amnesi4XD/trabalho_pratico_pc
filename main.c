#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "interface.h"
#include "lista_encadeada.h"

#define DISPONIVEL 0
#define OCUPADO 1
#define NAO_ENCONTRADO 99

int tempo_jogo = 60;

No *lista = NULL;

pthread_mutex_t lock_pedidos;
pthread_mutex_t lock_temp;
pthread_mutex_t lock_bancadas;
pthread_mutex_t lock_cozinhas;
Bancada *ingredientes;
Bancada *cozinhas;
Cozinheiro *cozinheiros;

struct Configuracao
{
  int qtd_cozinheiros;
  int qtd_bancadas;
  int qtd_cozinhas;
  int pontuacao_total;
  No *pedidos;
  WINDOW *tela_pedidos;
} configuracao;

int busca_bancada_disponivel()
{
  pthread_mutex_lock(&lock_bancadas);
  for (int i = 0; i < configuracao.qtd_bancadas; i++)
  {
    if (ingredientes[i].status == DISPONIVEL)
    {
      ingredientes[i].status = OCUPADO;
      pthread_mutex_unlock(&lock_bancadas);
      return i;
    }
  }
  pthread_mutex_unlock(&lock_bancadas);
  return NAO_ENCONTRADO;
}

int busca_cozinha_disponivel()
{
  pthread_mutex_lock(&lock_cozinhas);
  for (int i = 0; i < configuracao.qtd_cozinhas; i++)
  {
    if (cozinhas[i].status == DISPONIVEL)
    {
      cozinhas[i].status = OCUPADO;
      pthread_mutex_unlock(&lock_cozinhas);
      return i;
    }
  }
  pthread_mutex_unlock(&lock_cozinhas);
  return NAO_ENCONTRADO;
}

No *busca_no_pedido(No *no_atual, char *nome_do_pedido)
{
  while (no_atual != NULL)
  {
    if (!strcmp(no_atual->pedido, nome_do_pedido))
    {
      return no_atual;
    }
    no_atual = no_atual->proximo;
  }
  return NULL;
}

void *cozinhar(void **args)
{
  No *lista = (No *)args[0];
  Cozinheiro *cozinheiro = (Cozinheiro *)args[1];
  int indice_bancada_disponivel;
  int indice_cozinha_disponivel;
  No *pedido = busca_no_pedido(lista, cozinheiro->pedido_atual);

  while (indice_bancada_disponivel != DISPONIVEL)
  {
    indice_bancada_disponivel = busca_bancada_disponivel();
  }
  mvprintw(2, 4, (35 + indice_bancada_disponivel - 1) * 14 + (14 - strlen("Cozinheiro n")) / 2, "Cozinheiro");
  sleep(pedido->tempo_bancada);
  ingredientes[indice_bancada_disponivel].status = DISPONIVEL;

  while (indice_cozinha_disponivel != DISPONIVEL)
  {
    indice_cozinha_disponivel = busca_cozinha_disponivel();
  }

  sleep(pedido->tempo_cozinha);
  cozinhas[indice_cozinha_disponivel].status = DISPONIVEL;

  configuracao.pontuacao_total += pedido->pontos;

  remover(lista, pedido->pedido);

  return NULL;
}

void *adiciona_pedidos(void *arg)
{
  sleep(5);
  struct Configuracao *config = (struct Configuracao *)arg;
  No *lista = config->pedidos;

  while (1)
  {
    pthread_mutex_lock(&lock_pedidos);

    if (count(lista) < 14)
    {
      int pedido = (rand() % 5) + 1;
      switch (pedido)
      {
      case 1:
        lista = inserir(lista, "Hamburguer", 5, 10, 10);
        break;
      case 2:
        lista = inserir(lista, "Pizza", 4, 8, 10);
        break;
      case 3:
        lista = inserir(lista, "Salada", 4, 1, 6);
        break;
      case 4:
        lista = inserir(lista, "Suco", 2, 2, 5);
        break;
      case 5:
        lista = inserir(lista, "Macarrao", 6, 8, 8);
        break;
      default:
        break;
      }
    }

    atualizar_pedidos(config->tela_pedidos, lista);

    pthread_mutex_unlock(&lock_pedidos);

    int sleep_time = (rand() % 6) + 5;
    sleep(sleep_time);
  }

  return NULL;
}

void *temporizador(void *arg)
{
  WINDOW *temporizador = (WINDOW *)arg;
  int altura_tela, largura_tela;
  getmaxyx(stdscr, altura_tela, largura_tela);
  int altura_temp = 3;
  int largura_temp = 15;
  int starty = 1;
  int startx = largura_tela - largura_temp - 1;

  temporizador = newwin(altura_temp, largura_temp, starty, startx);
  box(temporizador, 0, 0);

  while (tempo_jogo > 0)
  {
    sleep(1);
    pthread_mutex_lock(&lock_temp);
    tempo_jogo--;
    pthread_mutex_unlock(&lock_temp);

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
  struct Configuracao *config = (struct Configuracao *)configuracao;

  for (int i = 1; i <= config->qtd_bancadas; i++)
  {
    cria_bancada(i);
  }

  for (int i = 1; i <= config->qtd_cozinhas; i++)
  {
    cria_cozinha(i);
  }
  lista_pedidos(config->tela_pedidos, config->pedidos);

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

  pthread_mutex_init(&lock_pedidos, NULL);
  pthread_mutex_init(&lock_temp, NULL);

  configuracao.pedidos = lista;

  initscr();
  raw();
  noecho();

  configuracao.tela_pedidos = newwin(30, 20, 2, 2);

  // Threads
  pthread_t thread_tela, thread_temporizador, thread_add_pedidos;

  // Criação das threads
  pthread_create(&thread_tela, NULL, interface, (void *)&configuracao);
  pthread_create(&thread_temporizador, NULL, temporizador, NULL);
  pthread_create(&thread_add_pedidos, NULL, adiciona_pedidos, (void *)&configuracao);

  pthread_join(thread_tela, NULL);
  pthread_join(thread_temporizador, NULL);
  pthread_join(thread_add_pedidos, NULL);

  // Aguarda o tempo do jogo
  while (tempo_jogo > 0)
  {
    if (tempo_jogo == 0)
    {
      endwin(); // Finaliza o NCurses
    }
  }

  return 0;
}
