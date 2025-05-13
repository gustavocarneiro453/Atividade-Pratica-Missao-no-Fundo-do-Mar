#ifndef FILA_JOGADORES_H
#define FILA_JOGADORES_H

// Nó da fila de jogadores
typedef struct NoJogador {
    char nome[50];
    struct NoJogador *prox;
} NoJogador;

// Estrutura da fila
typedef struct {
    NoJogador *inicio;
    NoJogador *fim;
} FilaJogadores;

// Inicializa a fila
void inicializarFila(FilaJogadores *fila);

// Verifica se a fila está vazia
int filaVazia(FilaJogadores *fila);

// Adiciona um jogador à fila
void enfileirar(FilaJogadores *fila, char nome[50]);

// Remove um jogador da fila (retorna o nome)
char* desenfileirar(FilaJogadores *fila);

// Retorna o tamanho da fila
int tamanhoFila(FilaJogadores *fila);

// Libera a memória da fila
void liberarFila(FilaJogadores *fila);

#endif 