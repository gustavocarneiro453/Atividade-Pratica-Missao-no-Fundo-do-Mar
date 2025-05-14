#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "objetos.h"

// Inicializa a lista de objetos
void inicializarListaObjetos(ListaObjetos *lista) {
    lista->inicio = NULL;
}

// Adiciona um objeto à lista
void adicionarObjeto(ListaObjetos *lista, int x, int y, int valor) {
    // Aloca memória para um novo objeto
    Objeto *novo = (Objeto*)malloc(sizeof(Objeto));
    
    if (novo != NULL) {
        // Inicializa os valores do objeto
        novo->x = x;
        novo->y = y;
        novo->valor = valor;
        novo->visivel = 1;  // Objeto visível inicialmente
        novo->tipo = 0;     // Por padrão é esmeralda (pode ser modificado depois)
        
        // Insere o objeto no início da lista
        novo->prox = lista->inicio;
        lista->inicio = novo;
    }
}

// Adiciona um objeto em posição aleatória
void adicionarObjetoAleatorio(ListaObjetos *lista, int max_x, int max_y) {
    // Gera posições aleatórias (evitando as bordas)
    int x = (rand() % (max_x - 2)) + 1;
    int y = (rand() % (max_y - 2)) + 1;
    
    // Adiciona o objeto com posição aleatória
    Objeto *novo = (Objeto*)malloc(sizeof(Objeto));
    
    if (novo != NULL) {
        // Inicializa os valores do objeto
        novo->x = x;
        novo->y = y;
        novo->visivel = 1;  // Objeto visível inicialmente
        
        // Tipo aleatório (0 a 4)
        novo->tipo = rand() % 5;
        
        // Define o valor com base no tipo
        switch (novo->tipo) {
            case 0: // Esmeralda
                novo->valor = 20;
                break;
            case 1: // Rubi
                novo->valor = 15;
                break;
            case 2: // Ouro
                novo->valor = 40;
                break;
            case 3: // Diamante
                novo->valor = 60;
                break;
            case 4: // Pérola
                novo->valor = 30;
                break;
            default:
                novo->valor = 10;
        }
        
        // Insere o objeto no início da lista
        novo->prox = lista->inicio;
        lista->inicio = novo;
    }
}

// Remove um objeto da lista
void removerObjeto(ListaObjetos *lista, int x, int y) {
    Objeto *atual = lista->inicio;
    Objeto *anterior = NULL;
    
    // Procura o objeto na lista
    while (atual != NULL) {
        if (atual->x == x && atual->y == y) {
            // Remove o objeto
            if (anterior == NULL) {
                // Objeto é o primeiro da lista
                lista->inicio = atual->prox;
            } else {
                // Objeto está no meio ou final da lista
                anterior->prox = atual->prox;
            }
            
            // Libera a memória do objeto
            free(atual);
            return;
        }
        
        anterior = atual;
        atual = atual->prox;
    }
}

// Libera a memória da lista de objetos
void liberarListaObjetos(ListaObjetos *lista) {
    Objeto *atual = lista->inicio;
    Objeto *proximo;
    
    // Percorre a lista liberando cada objeto
    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    
    lista->inicio = NULL;
}

// Verifica se um objeto está na posição (x,y)
Objeto* objetoEmPosicao(ListaObjetos *lista, int x, int y) {
    Objeto *atual = lista->inicio;
    
    // Procura um objeto na posição especificada
    while (atual != NULL) {
        if (atual->visivel && atual->x == x && atual->y == y) {
            return atual;
        }
        atual = atual->prox;
    }
    
    return NULL;  // Nenhum objeto encontrado
}

// Retorna o número de objetos na lista
int quantidadeObjetos(ListaObjetos *lista) {
    Objeto *atual = lista->inicio;
    int contador = 0;
    
    // Conta os objetos visíveis na lista
    while (atual != NULL) {
        if (atual->visivel) {
            contador++;
        }
        atual = atual->prox;
    }
    
    return contador;
} 