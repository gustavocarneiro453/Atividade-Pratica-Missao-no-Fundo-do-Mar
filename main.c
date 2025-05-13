#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "submarino.h"
#include "objetos.h"
#include "ranking.h"
#include "fila_jogadores.h"

#define LARGURA 70
#define ALTURA 20
#define TEMPO_JOGO 60

// Função para posicionar o cursor em uma coordenada específica
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Esconde o cursor do console
void esconderCursor() {
    CONSOLE_CURSOR_INFO cursor = {1, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// Mostra o cursor do console
void mostrarCursor() {
    CONSOLE_CURSOR_INFO cursor = {1, TRUE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// Limpa a tela sem causar o piscar
void limparTela() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD written;
    
    GetConsoleScreenBufferInfo(handle, &info);
    FillConsoleOutputCharacter(handle, ' ', info.dwSize.X * info.dwSize.Y, (COORD){0, 0}, &written);
    FillConsoleOutputAttribute(handle, info.wAttributes, info.dwSize.X * info.dwSize.Y, (COORD){0, 0}, &written);
    SetConsoleCursorPosition(handle, (COORD){0, 0});
}

// Desenha o cenário do jogo em um buffer
char cenarioBuffer[ALTURA][LARGURA];

void desenharCenario(Submarino *submarino, ListaObjetos *objetos) {
    int i, j;
    
    // Limpa o buffer
    for (i = 0; i < ALTURA; i++) {
        for (j = 0; j < LARGURA; j++) {
            if (i == 0 || i == ALTURA - 1 || j == 0 || j == LARGURA - 1)
                cenarioBuffer[i][j] = '#'; // Bordas
            else
                cenarioBuffer[i][j] = ' '; // Espaço vazio
        }
    }
    
    // Adiciona o submarino ao buffer
    cenarioBuffer[submarino->y][submarino->x] = 'S';
    
    // Adiciona os objetos ao buffer
    Objeto *atual = objetos->inicio;
    while (atual != NULL) {
        if (atual->visivel)
            cenarioBuffer[atual->y][atual->x] = 'O';
        atual = atual->prox;
    }
}

// Exibe o cenário na tela atualizando apenas o necessário
void exibirCenario(int tempo, int pontos) {
    int i, j;
    static char ultimoCenario[ALTURA][LARGURA] = {0};
    
    // Atualiza apenas as informações do cabeçalho
    gotoxy(0, 0);
    printf("Missão no Fundo do Mar - Tempo: %d segundos - Pontuação: %d    ", tempo, pontos);
    
    // Atualiza apenas o que mudou no cenário
    for (i = 0; i < ALTURA; i++) {
        for (j = 0; j < LARGURA; j++) {
            if (cenarioBuffer[i][j] != ultimoCenario[i][j]) {
                gotoxy(j, i+1); // +1 para pular a linha do cabeçalho
                printf("%c", cenarioBuffer[i][j]);
                ultimoCenario[i][j] = cenarioBuffer[i][j];
            }
        }
    }
    
    // Atualiza as instruções apenas na primeira vez
    static int primeiraVez = 1;
    if (primeiraVez) {
        gotoxy(0, ALTURA+1);
        printf("\nUse as teclas W, A, S, D para mover o submarino.");
        gotoxy(0, ALTURA+2);
        printf("Colete os objetos 'O' para ganhar pontos.");
        primeiraVez = 0;
    }
}

// Verifica colisão entre submarino e objetos
void verificarColisao(Submarino *submarino, ListaObjetos *objetos, int *pontos) {
    Objeto *atual = objetos->inicio;
    
    while (atual != NULL) {
        if (atual->visivel && atual->x == submarino->x && atual->y == submarino->y) {
            *pontos += atual->valor;
            atual->visivel = 0; // Esconde o objeto coletado
            
            // Gera um novo objeto aleatório
            adicionarObjetoAleatorio(objetos, LARGURA, ALTURA);
        }
        atual = atual->prox;
    }
}

// Função principal do jogo
void jogar(char nome[50]) {
    Submarino submarino;
    ListaObjetos objetos;
    int pontos = 0;
    int tecla;
    time_t inicio, atual;
    int tempo_restante;
    
    // Limpa a tela e esconde o cursor
    limparTela();
    esconderCursor();
    
    // Inicializa o submarino
    submarino.x = LARGURA / 2;
    submarino.y = ALTURA / 2;
    
    // Inicializa a lista de objetos
    inicializarListaObjetos(&objetos);
    
    // Adiciona objetos iniciais
    int i;
    for (i = 0; i < 10; i++) {
        adicionarObjetoAleatorio(&objetos, LARGURA, ALTURA);
    }
    
    // Desenha o cenário inicial
    desenharCenario(&submarino, &objetos);
    
    // Marca o tempo inicial
    time(&inicio);
    
    while (1) {
        // Calcula o tempo restante
        time(&atual);
        tempo_restante = TEMPO_JOGO - (int)(atual - inicio);
        
        if (tempo_restante <= 0) {
            break; // Fim do jogo por tempo
        }
        
        // Desenha e exibe o cenário
        desenharCenario(&submarino, &objetos);
        exibirCenario(tempo_restante, pontos);
        
        // Verifica entrada do teclado (sem bloquear)
        if (_kbhit()) {
            tecla = _getch();
            
            // Atualiza posição do submarino
            switch (tecla) {
                case 'w': case 'W':
                    if (submarino.y > 1) submarino.y--;
                    break;
                case 's': case 'S':
                    if (submarino.y < ALTURA - 2) submarino.y++;
                    break;
                case 'a': case 'A':
                    if (submarino.x > 1) submarino.x--;
                    break;
                case 'd': case 'D':
                    if (submarino.x < LARGURA - 2) submarino.x++;
                    break;
                case 27: // ESC para sair
                    tempo_restante = 0;
                    break;
            }
        }
        
        // Verifica colisões
        verificarColisao(&submarino, &objetos, &pontos);
        
        // Pequena pausa para controlar a velocidade do jogo
        Sleep(50);
    }
    
    // Fim do jogo
    mostrarCursor();
    limparTela();
    printf("Fim de jogo!\n");
    printf("Jogador: %s\n", nome);
    printf("Pontuação final: %d\n", pontos);
    printf("\nPressione qualquer tecla para continuar...");
    _getch();
    
    // Adiciona a pontuação ao ranking
    adicionarAoRanking(nome, pontos);
    
    // Libera a memória da lista de objetos
    liberarListaObjetos(&objetos);
}

// Menu principal
void menuPrincipal() {
    int opcao;
    char nome[50];
    
    // Inicializa o ranking
    inicializarRanking();
    
    // Inicializa a fila de jogadores
    FilaJogadores fila;
    inicializarFila(&fila);
    
    do {
        mostrarCursor(); // Mostra o cursor para navegação no menu
        limparTela();
        printf("==== MISSÃO NO FUNDO DO MAR ====\n\n");
        printf("1. Jogar\n");
        printf("2. Entrar na fila de jogadores\n");
        printf("3. Exibir ranking\n");
        printf("4. Sair\n\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                if (filaVazia(&fila)) {
                    limparTela();
                    printf("Digite seu nome: ");
                    scanf("%s", nome);
                    jogar(nome);
                } else {
                    // Pega o primeiro jogador da fila
                    strcpy(nome, desenfileirar(&fila));
                    printf("\nJogador %s da fila vai jogar!\n", nome);
                    printf("Pressione qualquer tecla para iniciar...");
                    _getch();
                    jogar(nome);
                }
                break;
                
            case 2:
                limparTela();
                printf("Digite seu nome para entrar na fila: ");
                scanf("%s", nome);
                enfileirar(&fila, nome);
                printf("\nJogador %s adicionado à fila!\n", nome);
                printf("Há %d jogadores na fila.\n", tamanhoFila(&fila));
                printf("\nPressione qualquer tecla para voltar ao menu...");
                _getch();
                break;
                
            case 3:
                limparTela();
                exibirRanking();
                printf("\nPressione qualquer tecla para voltar ao menu...");
                _getch();
                break;
                
            case 4:
                limparTela();
                printf("Obrigado por jogar!\n");
                break;
                
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                Sleep(1000);
        }
        
    } while (opcao != 4);
    
    // Libera a memória da fila
    liberarFila(&fila);
    
    // Libera a memória do ranking
    liberarRanking();
}

int main() {
    // Configura o console para exibição adequada
    system("mode con cols=80 lines=30");
    
    // Inicializa a semente para números aleatórios
    srand(time(NULL));
    
    // Inicia o menu principal
    menuPrincipal();
    
    return 0;
} 