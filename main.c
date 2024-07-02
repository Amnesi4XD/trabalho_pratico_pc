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

extern pthread_mutex_t lock_pedidos;
pthread_mutex_t lock_temp;
pthread_mutex_t lock_bancadas;
pthread_mutex_t lock_cozinhas;
pthread_mutex_t lock_ordem[3];
Bancada *ingredientes;
Bancada *cozinhas;
int ordem[3] = {0, 0, 0};
struct Configuracao
{
  int qtd_cozinheiros;
  int qtd_bancadas;
  int qtd_cozinhas;
  int pontuacao_total;
  WINDOW *tela_pedidos;
} configuracao;

typedef struct
{
  int id_cozinheiro;
  WINDOW *tela;
} Cozinheiro;

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

void *cozinheiros(void *args)
{
  int num_pedido;
  int indice_bancada_disponivel;
  int indice_cozinha_disponivel;
  int alturaTela, larguraTela;
  getmaxyx(stdscr,alturaTela, larguraTela);

  Cozinheiro *cozinheiro = (Cozinheiro *)args;
  while (1)
  {
    for (int i = 0; i < 3; i++)
    {
      pthread_mutex_lock(&lock_ordem[i]);
      if (cozinheiro->id_cozinheiro == i && ordem[i] != 0)
      {
        num_pedido = ordem[i];


        pthread_mutex_lock(&lock_pedidos);
        No *pedido = busca_no_pedido(lista, num_pedido);
        pthread_mutex_unlock(&lock_pedidos);

        while (indice_bancada_disponivel != DISPONIVEL)
        {
          indice_bancada_disponivel = busca_bancada_disponivel();
        }
        mvprintw(4, 35 + (indice_bancada_disponivel) - 1 * 15, "Cozinheiro %d", cozinheiro->id_cozinheiro);
        refresh();
        sleep(pedido->tempo_bancada);
        mvprintw(4, 35 + (indice_bancada_disponivel - 1) * 15, "             ");
        ingredientes[indice_bancada_disponivel].status = DISPONIVEL;

        while (indice_cozinha_disponivel != DISPONIVEL)
        {
          indice_cozinha_disponivel = busca_cozinha_disponivel();
        }
        mvprintw(alturaTela - 13 + 5 / 2, 35 + (indice_cozinha_disponivel - 1) * 15, "Cozinheiro %d", cozinheiro->id_cozinheiro);
        sleep(pedido->tempo_cozinha);
        mvprintw(alturaTela - 13 + 5 / 2, 35 + (indice_cozinha_disponivel - 1) * 15, "             ");
        cozinhas[indice_cozinha_disponivel].status = DISPONIVEL;

        configuracao.pontuacao_total += pedido->pontos;

        pthread_mutex_lock(&lock_pedidos);
        remover(lista, num_pedido);


        atualizar_pedidos(cozinheiro->tela, lista);
        pthread_mutex_unlock(&lock_pedidos);

        ordem[i] = 0;
        pthread_mutex_unlock(&lock_ordem[i]);

        return NULL;
      }
      else
      {
        pthread_mutex_unlock(&lock_ordem[i]);
      }
    }
  }
}

void *gerente(void *arg)
{
  struct Configuracao *config = (struct Configuracao *)arg;

  char input;
  int input_num;
  int buffer[BUFFER_SIZE] = {0};
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
    } while (input_num <= 0 || input_num >= count(lista));
    buffer[1] = input_num;

    switch (buffer[0])
    {
    case 1:
    {
      pthread_mutex_lock(&lock_ordem[0]);
      ordem[0] = buffer[1];
      pthread_mutex_unlock(&lock_ordem[0]);
    }
    break;
    case 2:
    {
      pthread_mutex_lock(&lock_ordem[1]);
      ordem[1] = buffer[1];
      pthread_mutex_unlock(&lock_ordem[1]);
    }
    case 3:
    {
      pthread_mutex_lock(&lock_ordem[2]);
      ordem[2] = buffer[1];
      pthread_mutex_unlock(&lock_ordem[2]);
    }
    break;
    }

    return NULL;
  }
}

void *adiciona_pedidos(void *arg)
{
  sleep(2);
  struct Configuracao *config = (struct Configuracao *)arg;
  while (1)
  {
    pthread_mutex_lock(&lock_pedidos);
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
      atualizar_pedidos(config->tela_pedidos, lista);
    }
    pthread_mutex_unlock(&lock_pedidos);

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
  lista_pedidos(config->tela_pedidos);
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

  ingredientes = (Bancada *)malloc(configuracao.qtd_bancadas * sizeof(Bancada));
  for (int i = 0; i < configuracao.qtd_bancadas; i++)
  {
    ingredientes[i].status = DISPONIVEL;
  }

  cozinhas = (Bancada *)malloc(configuracao.qtd_cozinhas * sizeof(Bancada));
  for (int i = 0; i < configuracao.qtd_cozinhas; i++)
  {
    cozinhas[i].status = DISPONIVEL;
  }

  pthread_mutex_init(&lock_pedidos, NULL);
  pthread_mutex_init(&lock_temp, NULL);
  pthread_mutex_init(&lock_bancadas, NULL);
  pthread_mutex_init(&lock_cozinhas, NULL);
  for (int i = 0; i < 3; i++) 
  {
    pthread_mutex_init(&lock_ordem[i], NULL);
  }

  initscr();
  raw();
  noecho();

  configuracao.tela_pedidos = newwin(30, 20, 2, 2);

  pthread_t thread_tela, thread_temporizador, thread_add_pedidos, thread_gerente, thread_cozinheiro[3];

  pthread_create(&thread_tela, NULL, interface, (void *)&configuracao);
  pthread_create(&thread_temporizador, NULL, temporizador, NULL);
  pthread_create(&thread_add_pedidos, NULL, adiciona_pedidos, (void *)&configuracao);
  pthread_create(&thread_gerente, NULL, gerente, (void *)&configuracao);
  for (int i = 0; i < qtd_cozinheiros; i++)
  {
    Cozinheiro *cozinheiro = (Cozinheiro *)malloc(sizeof(Cozinheiro));
    cozinheiro->id_cozinheiro = i + 1;
    cozinheiro->tela = configuracao.tela_pedidos; // para ser capaz de atualizar a tela de pedidos
    pthread_create(&thread_cozinheiro[i], NULL, cozinheiros, (void *)&cozinheiro);
  }

  pthread_join(thread_tela, NULL);
  pthread_join(thread_temporizador, NULL);
  pthread_join(thread_add_pedidos, NULL);
  for (int i = 0; i < qtd_cozinheiros; i++)
  {
    pthread_join(thread_cozinheiro[i], NULL);
  }


  while (tempo_jogo >= 0)
  {
    if (tempo_jogo == 0)
    {
      endwin(); // Finaliza o NCurses
    }
  }

  free(ingredientes);
  free(cozinhas);

  pthread_mutex_destroy(&lock_pedidos);
  pthread_mutex_destroy(&lock_bancadas);
  pthread_mutex_destroy(&lock_cozinhas);
  pthread_mutex_destroy(&lock_pedidos);
  for (int i = 0; i < 3; i++) 
  {
    pthread_mutex_destroy(&lock_ordem[i]);
  }

  return 0;
}
