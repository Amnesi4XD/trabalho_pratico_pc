#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "interface.h"
#include "lista_encadeada.h"

#define DISPONIVEL 0
#define OCUPADO 1
#define NAO_ENCONTRADO 99
#define BUFFER_SIZE 2

int tempo_jogo = 60;

No *lista = NULL;

pthread_mutex_t lock_pedidos;
pthread_mutex_t lock_temp;
pthread_mutex_t lock_bancadas;
pthread_mutex_t lock_cozinhas;
Bancada *ingredientes;
Bancada *cozinhas;
pthread_mutex_t *lock_cozinheiros;

struct Configuracao
{
  int qtd_cozinheiros;
  int qtd_bancadas;
  int qtd_cozinhas;
  int pontuacao_total;
  No *pedidos;
  WINDOW *tela_pedidos;
} configuracao;

typedef struct
{
  No *lista_pedidos;
  int num_pedido;
  WINDOW *tela;
} ArgsThread;

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

No *busca_no_pedido(No *no_atual, int indice_do_pedido)
{
  int contador = 1;

  while (no_atual != NULL)
  {
    if (contador == indice_do_pedido)
    {
      return no_atual;
    }
    no_atual = no_atual->proximo;
    contador++;
  }
  return NULL;
}

void *cozinhar(void *args)
{
  ArgsThread *argumentos = (ArgsThread *)args;
  No *lista = argumentos->lista_pedidos;
  int num_pedido = argumentos->num_pedido;
  WINDOW *tela = argumentos->tela;

  int indice_bancada_disponivel;
  int indice_cozinha_disponivel;

  pthread_mutex_lock(&lock_pedidos);
  No *pedido = busca_no_pedido(lista, num_pedido);
  pthread_mutex_unlock(&lock_pedidos);

  while (indice_bancada_disponivel != DISPONIVEL)
  {
    indice_bancada_disponivel = busca_bancada_disponivel();
  }

  sleep(pedido->tempo_bancada);
  ingredientes[indice_bancada_disponivel].status = DISPONIVEL;

  while (indice_cozinha_disponivel != DISPONIVEL)
  {
    indice_cozinha_disponivel = busca_cozinha_disponivel();
  }

  sleep(pedido->tempo_cozinha);
  cozinhas[indice_cozinha_disponivel].status = DISPONIVEL;

  configuracao.pontuacao_total += pedido->pontos;

  pthread_mutex_lock(&lock_pedidos);
  remover(lista, pedido->pedido);
  pthread_mutex_unlock(&lock_pedidos);

  atualizar_pedidos(tela, lista);

  free(argumentos);
  return NULL;
}

void *gerente(void *arg)
{
  struct Configuracao *config = (struct Configuracao *)arg;

  char input;
  int input_num;
  int buffer[BUFFER_SIZE] = {0};
  int buffer_index = 0;

  pthread_t *cozinheiro_thread = malloc(config->qtd_cozinheiros * sizeof(pthread_t));
  pthread_mutex_t *lock_cozinheiros = malloc(config->qtd_cozinheiros * sizeof(pthread_mutex_t));

  for (int i = 0; i < config->qtd_cozinheiros; i++)
  {
    pthread_mutex_init(&lock_cozinheiros[i], NULL);
  }

  while (1)
  {
    do
    {
      input = getch();
      input_num = atoi(&input);
      refresh();
    } while (input_num <= 0 || input_num > config->qtd_cozinheiros);
    buffer[0] = input_num;

    do
    {
      input = getch();
      input_num = atoi(&input);
      refresh();
    } while (input_num <= 0 || input_num >= count(config->pedidos));
    buffer[1] = input_num;

    switch (buffer[0])
    {
    case 1:
    {
      mvprintw(10, 20, "Cozinheiro 1 selecionado");

      ArgsThread *args = malloc(sizeof(ArgsThread));
      args->lista_pedidos = config->pedidos;
      args->num_pedido = buffer[1];
      args->tela = config->tela_pedidos;

      pthread_mutex_lock(&lock_cozinheiros[0]);
      pthread_create(&cozinheiro_thread[0], NULL, cozinhar, args);
      pthread_mutex_unlock(&lock_cozinheiros[0]);
    }
    break;
    case 2:
    {
      mvprintw(10, 20, "Cozinheiro 2 selecionado");

      ArgsThread *args = malloc(sizeof(ArgsThread));
      args->lista_pedidos = config->pedidos;
      args->num_pedido = buffer[1];
      args->tela = config->tela_pedidos;

      pthread_mutex_lock(&lock_cozinheiros[1]);
      pthread_create(&cozinheiro_thread[1], NULL, cozinhar, args);
      pthread_mutex_unlock(&lock_cozinheiros[1]);
    }
    break;
    case 3:
    {
      mvprintw(10, 20, "Cozinheiro 3 selecionado");

      ArgsThread *args = malloc(sizeof(ArgsThread));
      args->lista_pedidos = config->pedidos;
      args->num_pedido = buffer[1];
      args->tela = config->tela_pedidos;

      pthread_mutex_lock(&lock_cozinheiros[2]);
      pthread_create(&cozinheiro_thread[2], NULL, cozinhar, args);
      pthread_mutex_unlock(&lock_cozinheiros[2]);
    }
    break;
    }

    free(cozinheiro_thread);
    free(lock_cozinheiros);

    return NULL;
  }
}

void *adiciona_pedidos(void *arg)
{
  sleep(2);
  struct Configuracao *config = (struct Configuracao *)arg;
  No *lista = config->pedidos;

  while (1)
  {
    if (count(lista) < 28)
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
      pthread_mutex_lock(&lock_pedidos);
      config->pedidos = lista;
      pthread_mutex_unlock(&lock_pedidos);

      refresh();
    }

    atualizar_pedidos(config->tela_pedidos, lista);

    int sleep_time = (rand() % 6) + 3;
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

  cria_box_cozinheiros(config->qtd_cozinheiros);

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
  int qtd_cozinheiros, qtd_bancadas, qtd_cozinhas;

  do
  {
    printf("Informe a quantidade de cozinheiros (max 3): ");
    scanf("%d", &qtd_cozinheiros);
    if (qtd_cozinheiros <= 0 || qtd_cozinheiros > 3)
    {
      printf("Número de cozinheiros inválido. Tente novamente.\n");
    }
  } while (qtd_cozinheiros <= 0 || qtd_cozinheiros > 3);

  do
  {
    printf("\nInforme a quantidade de bancadas (max 3): ");
    scanf("%d", &qtd_bancadas);
    if (qtd_bancadas <= 0 || qtd_bancadas > 3)
    {
      printf("Número de bancadas inválido. Tente novamente.\n");
    }
  } while (qtd_bancadas <= 0 || qtd_bancadas > 3);

  do
  {
    printf("\nInforme a quantidade de cozinhas (max 3): ");
    scanf("%d", &qtd_cozinhas);
    if (qtd_cozinhas <= 0 || qtd_cozinhas > 3)
    {
      printf("Número de cozinhas inválido. Tente novamente.\n");
    }
  } while (qtd_cozinhas <= 0 || qtd_cozinhas > 3);

  configuracao.qtd_cozinheiros = qtd_cozinheiros;
  configuracao.qtd_cozinhas = qtd_cozinhas;
  configuracao.qtd_bancadas = qtd_bancadas;

  initscr();
  raw();
  noecho();

  configuracao.tela_pedidos = newwin(30, 20, 2, 2);

  pthread_t thread_tela, thread_temporizador, thread_add_pedidos, thread_gerente;

  pthread_create(&thread_tela, NULL, interface, (void *)&configuracao);
  pthread_create(&thread_temporizador, NULL, temporizador, NULL);
  pthread_create(&thread_add_pedidos, NULL, adiciona_pedidos, (void *)&configuracao);
  pthread_create(&thread_gerente, NULL, gerente, (void *)&configuracao);

  pthread_join(thread_tela, NULL);
  pthread_join(thread_temporizador, NULL);
  pthread_join(thread_add_pedidos, NULL);

  while (tempo_jogo >= 0)
  {
    if (tempo_jogo == 0)
    {
      endwin(); // Finaliza o NCurses
    }
  }

  return 0;
}
