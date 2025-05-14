#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"

// Raiz da árvore de ranking
static Ranking *raiz = NULL;

// Inicializa o ranking
void inicializarRanking() {
    // Limpa a árvore caso já exista
    liberarRanking();
    raiz = NULL;
}

// Função auxiliar para inserir um nó na árvore
Ranking* inserirNo(Ranking *no, char nome[50], int pontuacao) {
    // Se chegou a uma folha, cria um novo nó
    if (no == NULL) {
        Ranking *novo = (Ranking*)malloc(sizeof(Ranking));
        if (novo != NULL) {
            strcpy(novo->nome, nome);
            novo->pontuacao = pontuacao;
            novo->esq = NULL;
            novo->dir = NULL;
        }
        return novo;
    }
    
    // Insere de forma que a árvore fique ordenada por pontuação (maior para menor)
    if (pontuacao > no->pontuacao) {
        no->esq = inserirNo(no->esq, nome, pontuacao);
    } else {
        no->dir = inserirNo(no->dir, nome, pontuacao);
    }
    
    return no;
}

// Adiciona uma pontuação ao ranking
void adicionarAoRanking(char nome[50], int pontuacao) {
    raiz = inserirNo(raiz, nome, pontuacao);
}

// Função auxiliar para exibir ranking em ordem decrescente (in-ordem invertida)
void exibirRankingRecursivo(Ranking *no, int *posicao) {
    if (no == NULL) {
        return;
    }
    
    // Visita primeiro o lado esquerdo (maiores pontuações)
    exibirRankingRecursivo(no->esq, posicao);
    
    // Exibe o nó atual
    printf("%dº lugar: %s - %d pontos\n", *posicao, no->nome, no->pontuacao);
    (*posicao)++; // Incrementa a posição para o próximo jogador
    
    // Visita o lado direito (menores pontuações)
    exibirRankingRecursivo(no->dir, posicao);
}

// Exibe o ranking em ordem decrescente
void exibirRanking() {
    printf("===== RANKING =====\n\n");
    
    if (raiz == NULL) {
        printf("Nenhuma pontuação registrada ainda.\n");
    } else {
        int posicao = 1; // Inicializa a posição com 1 para cada exibição
        exibirRankingRecursivo(raiz, &posicao);
    }
}

// Função auxiliar para liberar a árvore recursivamente
void liberarRankingRecursivo(Ranking *no) {
    if (no != NULL) {
        liberarRankingRecursivo(no->esq);
        liberarRankingRecursivo(no->dir);
        free(no);
    }
}

// Libera a memória do ranking
void liberarRanking() {
    liberarRankingRecursivo(raiz);
    raiz = NULL;
}

// Função auxiliar para converter a árvore em um array (para ordenação)
void arvoreParaArray(Ranking *no, Ranking *array[], int *indice) {
    if (no != NULL) {
        arvoreParaArray(no->esq, array, indice);
        array[(*indice)++] = no;
        arvoreParaArray(no->dir, array, indice);
    }
}

// Função de particionamento para o QuickSort
int particionar(Ranking *array[], int inicio, int fim) {
    int pivot = inicio;
    int indice = fim;
    
    for (int i = fim; i > inicio; i--) {
        if (array[i]->pontuacao >= array[pivot]->pontuacao) {
            // Troca os elementos
            Ranking *temp = array[i];
            array[i] = array[indice];
            array[indice] = temp;
            indice--;
        }
    }
    
    // Troca o pivô para sua posição correta
    Ranking *temp = array[pivot];
    array[pivot] = array[indice];
    array[indice] = temp;
    
    return indice;
}

// Implementação do QuickSort para ordenar o ranking
void quickSort(Ranking *array[], int inicio, int fim) {
    if (inicio < fim) {
        int indice = particionar(array, inicio, fim);
        
        quickSort(array, inicio, indice - 1);
        quickSort(array, indice + 1, fim);
    }
}

// Função para ordenar o ranking usando QuickSort
void ordenarRanking() {
    // Contar quantos nós existem na árvore
    int tamanho = 0;
    contarNos(raiz, &tamanho);
    
    if (tamanho == 0) return;
    
    // Criar um array para armazenar os nós
    Ranking **array = (Ranking**)malloc(tamanho * sizeof(Ranking*));
    if (array == NULL) return;
    
    // Preencher o array com os nós da árvore
    int indice = 0;
    arvoreParaArray(raiz, array, &indice);
    
    // Ordenar o array usando QuickSort
    quickSort(array, 0, tamanho - 1);
    
    // Reconstruir a árvore a partir do array ordenado
    // (Isso não é implementado aqui, pois a árvore já está
    // ordenada pela forma como inserimos os nós)
    
    free(array);
}

// Função auxiliar para contar nós na árvore
void contarNos(Ranking *no, int *contador) {
    if (no != NULL) {
        (*contador)++;
        contarNos(no->esq, contador);
        contarNos(no->dir, contador);
    }
} 