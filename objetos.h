#ifndef OBJETOS_H
#define OBJETOS_H

// Estrutura de um objeto coletável
typedef struct Objeto {
    int x;          // Posição X
    int y;          // Posição Y
    int valor;      // Valor do objeto (pontos)
    int visivel;    // 1 = visível, 0 = coletado/não visível
    struct Objeto *prox;  // Próximo objeto na lista
} Objeto;

// Estrutura da lista de objetos
typedef struct {
    Objeto *inicio;  // Primeiro objeto da lista
} ListaObjetos;

// Inicializa a lista de objetos
void inicializarListaObjetos(ListaObjetos *lista);

// Adiciona um objeto à lista
void adicionarObjeto(ListaObjetos *lista, int x, int y, int valor);

// Adiciona um objeto em posição aleatória
void adicionarObjetoAleatorio(ListaObjetos *lista, int max_x, int max_y);

// Remove um objeto da lista
void removerObjeto(ListaObjetos *lista, int x, int y);

// Libera a memória da lista de objetos
void liberarListaObjetos(ListaObjetos *lista);

// Verifica se um objeto está na posição (x,y)
Objeto* objetoEmPosicao(ListaObjetos *lista, int x, int y);

// Retorna o número de objetos na lista
int quantidadeObjetos(ListaObjetos *lista);

#endif 