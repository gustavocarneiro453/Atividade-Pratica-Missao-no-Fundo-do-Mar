#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila_jogadores.h"

// Inicializa a fila
void inicializarFila(FilaJogadores *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
}

// Verifica se a fila está vazia
int filaVazia(FilaJogadores *fila) {
    return (fila->inicio == NULL);
}

// Adiciona um jogador à fila
void enfileirar(FilaJogadores *fila, char nome[50]) {
    NoJogador *novo = (NoJogador*)malloc(sizeof(NoJogador));
    
    if (novo != NULL) {
        strcpy(novo->nome, nome);
        novo->prox = NULL;
        
        // Se a fila estiver vazia, o novo elemento é o primeiro e o último
        if (filaVazia(fila)) {
            fila->inicio = novo;
            fila->fim = novo;
        } else {
            // Adiciona o novo elemento ao final da fila
            fila->fim->prox = novo;
            fila->fim = novo;
        }
    }
}

// Remove um jogador da fila (retorna o nome)
char* desenfileirar(FilaJogadores *fila) {
    static char nome[50]; // Variável estática para armazenar o nome
    
    if (!filaVazia(fila)) {
        NoJogador *temp = fila->inicio;
        
        // Salva o nome antes de liberar a memória
        strcpy(nome, temp->nome);
        
        // Move o início da fila para o próximo elemento
        fila->inicio = fila->inicio->prox;
        
        // Se a fila ficar vazia, atualiza o fim também
        if (fila->inicio == NULL) {
            fila->fim = NULL;
        }
        
        // Libera a memória do nó removido
        free(temp);
        
        return nome;
    }
    
    return "";
}

// Retorna o tamanho da fila
int tamanhoFila(FilaJogadores *fila) {
    int contador = 0;
    NoJogador *atual = fila->inicio;
    
    while (atual != NULL) {
        contador++;
        atual = atual->prox;
    }
    
    return contador;
}

// Libera a memória da fila
void liberarFila(FilaJogadores *fila) {
    NoJogador *atual = fila->inicio;
    NoJogador *proximo;
    
    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    
    fila->inicio = NULL;
    fila->fim = NULL;
} 