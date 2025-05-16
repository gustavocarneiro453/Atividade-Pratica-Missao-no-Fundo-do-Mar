CC = gcc
CFLAGS = -Wall -I. -I./include -I./curl/include
LDFLAGS = -L./curl/lib -lcurl -lraylib -lm -lgdi32 -lwinmm

SRCS = raylib_game.c submarino.c objetos.c ranking.c fila_jogadores.c ia_helper.c
OBJS = $(SRCS:.c=.o)
TARGET = jogo_submarino

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del *.o $(TARGET).exe 