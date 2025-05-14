#ifndef RANKING_H
#define RANKING_H

// Nó da árvore de ranking
typedef struct Ranking {
    char nome[50];
    int pontuacao;
    struct Ranking *esq;
    struct Ranking *dir;
} Ranking;

// Inicializa o ranking
void inicializarRanking();

// Adiciona uma pontuação ao ranking
void adicionarAoRanking(char nome[50], int pontuacao);

// Exibe o ranking em ordem decrescente
void exibirRanking();

// Libera a memória do ranking
void liberarRanking();

// Ordena o ranking usando QuickSort
void ordenarRanking();

// Funções auxiliares (não expostas no header)
void exibirRankingRecursivo(Ranking *raiz, int *posicao);
void liberarRankingRecursivo(Ranking *raiz);
void contarNos(Ranking *no, int *contador);

#endif 