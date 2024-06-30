# Nome do programa principal
PROGRAMA = main

# Arquivos fonte em C
SRC_C = main.c lista_encadeada.c interface.c

# Arquivos cabeçalho em C
HDR_C = lista_encadeada.h interface.h

# Objetos
OBJS = $(SRC_C:.c=.o)

# Flags para o compilador
CFLAGS = -Wall -std=c99

# Regras de compilação
$(OBJS): %.o: %.c $(HDR_C)
	gcc $(CFLAGS) -c $< -o $@

# Regra para o programa principal
$(PROGRAMA): $(OBJS)
	gcc $(CFLAGS) $(OBJS) -o $(PROGRAMA) -lncurses

# Regra para o alvo 'all'
all: $(PROGRAMA)

# Regra para limpar os arquivos objeto e o programa principal
clean:
	rm -f $(OBJS) $(PROGRAMA)

# Regra para executar o programa principal
run:
	./$(PROGRAMA)
