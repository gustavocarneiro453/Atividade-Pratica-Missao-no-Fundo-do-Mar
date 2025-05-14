#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"
#include "submarino.h"
#include "objetos.h"
#include "ranking.h"
#include "fila_jogadores.h"

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define LARGURA_JOGO 70
#define ALTURA_JOGO 20
#define TAMANHO_CELULA 25
#define TEMPO_JOGO 60

// Cores do jogo
#define COR_FUNDO (Color){ 12, 24, 36, 255 }  // Azul escuro para o fundo do mar
#define COR_BORDA (Color){ 0, 162, 232, 255 } // Azul para bordas
#define COR_SUBMARINO (Color){ 255, 255, 0, 255 } // Amarelo para o submarino
#define COR_OBJETO (Color){ 0, 255, 0, 255 }  // Verde para objetos
#define COR_TEXTO (Color){ 255, 255, 255, 255 } // Branco para texto

// Estruturas para elementos visuais
typedef struct {
    Texture2D textura;
    Rectangle origem;
} Sprite;

// Protótipo da função para exibir o ranking
void exibirRankingGrafico();

// Função para carregar texturas
void carregarTexturas(Sprite *submarinoSprite, Sprite *objetoSprite) {
    // Submarino (retângulo amarelo)
    Image imagemSubmarino = GenImageColor(TAMANHO_CELULA, TAMANHO_CELULA, YELLOW);
    submarinoSprite->textura = LoadTextureFromImage(imagemSubmarino);
    submarinoSprite->origem = (Rectangle){ 0, 0, TAMANHO_CELULA, TAMANHO_CELULA };
    UnloadImage(imagemSubmarino);
    
    // Objeto (círculo verde)
    Image imagemObjeto = GenImageColor(TAMANHO_CELULA, TAMANHO_CELULA, GREEN);
    objetoSprite->textura = LoadTextureFromImage(imagemObjeto);
    objetoSprite->origem = (Rectangle){ 0, 0, TAMANHO_CELULA, TAMANHO_CELULA };
    UnloadImage(imagemObjeto);
}

// Função para desenhar o cenário
void desenharCenario(Submarino *submarino, ListaObjetos *objetos, Sprite submarinoSprite, Sprite objetoSprite) {
    int i, j;
    int offsetX = (LARGURA_TELA - LARGURA_JOGO * TAMANHO_CELULA) / 2;
    int offsetY = (ALTURA_TELA - ALTURA_JOGO * TAMANHO_CELULA) / 2;
    
    // Desenha o fundo
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, COR_FUNDO);
    
    // Desenha a área do jogo
    for (i = 0; i < ALTURA_JOGO; i++) {
        for (j = 0; j < LARGURA_JOGO; j++) {
            // Desenha as bordas
            if (i == 0 || i == ALTURA_JOGO - 1 || j == 0 || j == LARGURA_JOGO - 1) {
                DrawRectangle(
                    offsetX + j * TAMANHO_CELULA, 
                    offsetY + i * TAMANHO_CELULA, 
                    TAMANHO_CELULA, 
                    TAMANHO_CELULA, 
                    COR_BORDA
                );
            }
        }
    }
    
    // Desenha o submarino
    DrawRectangle(
        offsetX + submarino->x * TAMANHO_CELULA,
        offsetY + submarino->y * TAMANHO_CELULA,
        TAMANHO_CELULA,
        TAMANHO_CELULA,
        COR_SUBMARINO
    );
    
    // Desenha os objetos
    Objeto *atual = objetos->inicio;
    while (atual != NULL) {
        if (atual->visivel) {
            DrawCircle(
                offsetX + atual->x * TAMANHO_CELULA + TAMANHO_CELULA/2,
                offsetY + atual->y * TAMANHO_CELULA + TAMANHO_CELULA/2,
                TAMANHO_CELULA/2 - 2,
                COR_OBJETO
            );
        }
        atual = atual->prox;
    }
    
    // Desenha as informações do jogo
    DrawText("Missão no Fundo do Mar", 20, 20, 20, COR_TEXTO);
}

// Verifica colisão entre submarino e objetos
void verificarColisao(Submarino *submarino, ListaObjetos *objetos, int *pontos) {
    Objeto *atual = objetos->inicio;
    
    while (atual != NULL) {
        if (atual->visivel && atual->x == submarino->x && atual->y == submarino->y) {
            *pontos += atual->valor;
            atual->visivel = 0; // Esconde o objeto coletado
            
            // Gera um novo objeto aleatório
            adicionarObjetoAleatorio(objetos, LARGURA_JOGO, ALTURA_JOGO);
        }
        atual = atual->prox;
    }
}

// Função principal do jogo
void jogar(char nome[50]) {
    Submarino submarino;
    ListaObjetos objetos;
    int pontos = 0;
    time_t inicio, atual;
    int tempo_restante;
    Sprite submarinoSprite, objetoSprite;
    
    // Inicializa o submarino
    submarino.x = LARGURA_JOGO / 2;
    submarino.y = ALTURA_JOGO / 2;
    
    // Inicializa a lista de objetos
    inicializarListaObjetos(&objetos);
    
    // Adiciona objetos iniciais
    int i;
    for (i = 0; i < 10; i++) {
        adicionarObjetoAleatorio(&objetos, LARGURA_JOGO, ALTURA_JOGO);
    }
    
    // Carrega as texturas
    carregarTexturas(&submarinoSprite, &objetoSprite);
    
    // Marca o tempo inicial
    time(&inicio);
    
    // Loop principal do jogo
    while (!WindowShouldClose() && tempo_restante > 0) {
        // Calcula o tempo restante
        time(&atual);
        tempo_restante = TEMPO_JOGO - (int)(atual - inicio);
        
        // Atualiza a posição do submarino com base nas teclas
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            if (submarino.y > 1) submarino.y--;
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            if (submarino.y < ALTURA_JOGO - 2) submarino.y++;
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            if (submarino.x > 1) submarino.x--;
        }
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            if (submarino.x < LARGURA_JOGO - 2) submarino.x++;
        }
        
        // Verifica colisões
        verificarColisao(&submarino, &objetos, &pontos);
        
        // Desenha o jogo
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Desenha o cenário do jogo
        desenharCenario(&submarino, &objetos, submarinoSprite, objetoSprite);
        
        // Desenha as informações do jogo
        DrawText(TextFormat("Tempo: %d segundos", tempo_restante), LARGURA_TELA - 200, 20, 20, COR_TEXTO);
        DrawText(TextFormat("Pontuação: %d", pontos), LARGURA_TELA - 200, 50, 20, COR_TEXTO);
        DrawText(TextFormat("Jogador: %s", nome), 20, 50, 20, COR_TEXTO);
        
        EndDrawing();
    }
    
    // Descarrega as texturas
    UnloadTexture(submarinoSprite.textura);
    UnloadTexture(objetoSprite.textura);
    
    // Fim do jogo - Tela de resultados
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawText("Fim de jogo!", LARGURA_TELA/2 - 100, ALTURA_TELA/2 - 50, 30, WHITE);
        DrawText(TextFormat("Jogador: %s", nome), LARGURA_TELA/2 - 100, ALTURA_TELA/2, 20, WHITE);
        DrawText(TextFormat("Pontuação final: %d", pontos), LARGURA_TELA/2 - 100, ALTURA_TELA/2 + 30, 20, WHITE);
        DrawText("Pressione ESC para voltar ao menu", LARGURA_TELA/2 - 180, ALTURA_TELA/2 + 80, 20, WHITE);
        
        EndDrawing();
        
        if (IsKeyPressed(KEY_ESCAPE)) break;
    }
    
    // Adiciona a pontuação ao ranking
    adicionarAoRanking(nome, pontos);
    
    // Libera a memória da lista de objetos
    liberarListaObjetos(&objetos);
}

// Menu principal com interface gráfica
void menuPrincipal() {
    int opcaoSelecionada = 0;
    int totalOpcoes = 4;
    char nome[50] = "";
    bool digitandoNome = false;
    bool emFila = false;
    
    // Inicializa o ranking
    inicializarRanking();
    
    // Inicializa a fila de jogadores
    FilaJogadores fila;
    inicializarFila(&fila);
    
    // Loop do menu principal
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(COR_FUNDO);
        
        // Título do jogo
        DrawText("MISSÃO NO FUNDO DO MAR", LARGURA_TELA/2 - 200, 100, 30, WHITE);
        
        // Opções do menu
        const char* opcoes[] = {
            "Jogar",
            "Entrar na fila de jogadores",
            "Exibir ranking",
            "Sair"
        };
        
        // Desenha as opções do menu
        for (int i = 0; i < totalOpcoes; i++) {
            if (i == opcaoSelecionada) {
                DrawText(opcoes[i], LARGURA_TELA/2 - 100, 200 + i*40, 20, YELLOW);
            } else {
                DrawText(opcoes[i], LARGURA_TELA/2 - 100, 200 + i*40, 20, WHITE);
            }
        }
        
        // Controle de navegação do menu
        if (IsKeyPressed(KEY_UP) && opcaoSelecionada > 0) {
            opcaoSelecionada--;
        }
        if (IsKeyPressed(KEY_DOWN) && opcaoSelecionada < totalOpcoes - 1) {
            opcaoSelecionada++;
        }
        
        // Campo para digitação do nome
        if (digitandoNome) {
            DrawRectangle(LARGURA_TELA/2 - 200, ALTURA_TELA/2, 400, 100, DARKGRAY);
            DrawText("Digite seu nome:", LARGURA_TELA/2 - 180, ALTURA_TELA/2 + 20, 20, WHITE);
            DrawText(nome, LARGURA_TELA/2 - 180, ALTURA_TELA/2 + 50, 20, WHITE);
            
            // Captura as teclas digitadas
            int key = GetKeyPressed();
            if (key >= 32 && key <= 125 && strlen(nome) < 49) {
                char c[2] = {(char)key, '\0'};
                strcat(nome, c);
            }
            else if (IsKeyPressed(KEY_BACKSPACE) && strlen(nome) > 0) {
                nome[strlen(nome) - 1] = '\0';
            }
            else if (IsKeyPressed(KEY_ENTER) && strlen(nome) > 0) {
                digitandoNome = false;
                if (emFila) {
                    enfileirar(&fila, nome);
                    nome[0] = '\0';
                    emFila = false;
                } else {
                    jogar(nome);
                    nome[0] = '\0';
                }
            }
        }
        
        // Ação quando Enter é pressionado
        if (IsKeyPressed(KEY_ENTER) && !digitandoNome) {
            switch (opcaoSelecionada) {
                case 0: // Jogar
                    if (filaVazia(&fila)) {
                        digitandoNome = true;
                    } else {
                        strcpy(nome, desenfileirar(&fila));
                        jogar(nome);
                        nome[0] = '\0';
                    }
                    break;
                    
                case 1: // Entrar na fila
                    digitandoNome = true;
                    emFila = true;
                    break;
                    
                case 2: // Exibir ranking
                    exibirRankingGrafico();
                    break;
                    
                case 3: // Sair
                    CloseWindow();
                    break;
            }
        }
        
        EndDrawing();
    }
    
    // Libera a memória da fila
    liberarFila(&fila);
    
    // Libera a memória do ranking
    liberarRanking();
}

// Função para exibir o ranking com interface gráfica
void exibirRankingGrafico() {
    // Obtém o ranking ordenado (precisamos de uma função auxiliar)
    char nomes[10][50]; // Assumindo máximo de 10 jogadores no ranking
    int pontuacoes[10];
    int totalJogadores = 0;
    
    // Chamamos a função exibirRanking() com uma flag para não exibir no console
    // mas preencher nossos arrays (implemente isso se necessário)
    
    // Por enquanto, vamos simular o ranking
    FILE *arquivo = fopen("ranking.dat", "rb");
    if (arquivo != NULL) {
        Ranking jogador;
        while (fread(&jogador, sizeof(Ranking), 1, arquivo) == 1 && totalJogadores < 10) {
            strcpy(nomes[totalJogadores], jogador.nome);
            pontuacoes[totalJogadores] = jogador.pontuacao;
            totalJogadores++;
        }
        fclose(arquivo);
    }
    
    // Ordenar os jogadores por pontuação (bubble sort simples)
    for (int i = 0; i < totalJogadores - 1; i++) {
        for (int j = 0; j < totalJogadores - i - 1; j++) {
            if (pontuacoes[j] < pontuacoes[j + 1]) {
                // Troca pontuações
                int tempPontuacao = pontuacoes[j];
                pontuacoes[j] = pontuacoes[j + 1];
                pontuacoes[j + 1] = tempPontuacao;
                
                // Troca nomes
                char tempNome[50];
                strcpy(tempNome, nomes[j]);
                strcpy(nomes[j], nomes[j + 1]);
                strcpy(nomes[j + 1], tempNome);
            }
        }
    }
    
    // Loop de exibição do ranking
    bool sair = false;
    while (!sair && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(COR_FUNDO);
        
        DrawText("RANKING DE JOGADORES", LARGURA_TELA/2 - 180, 50, 30, WHITE);
        
        // Desenha as linhas do ranking
        if (totalJogadores == 0) {
            DrawText("Nenhum jogador no ranking!", LARGURA_TELA/2 - 150, ALTURA_TELA/2, 20, WHITE);
        } else {
            DrawText("Posição", LARGURA_TELA/2 - 250, 120, 20, YELLOW);
            DrawText("Nome", LARGURA_TELA/2 - 150, 120, 20, YELLOW);
            DrawText("Pontuação", LARGURA_TELA/2 + 100, 120, 20, YELLOW);
            
            for (int i = 0; i < totalJogadores; i++) {
                DrawText(TextFormat("%d.", i+1), LARGURA_TELA/2 - 250, 160 + i*30, 20, WHITE);
                DrawText(nomes[i], LARGURA_TELA/2 - 150, 160 + i*30, 20, WHITE);
                DrawText(TextFormat("%d", pontuacoes[i]), LARGURA_TELA/2 + 100, 160 + i*30, 20, WHITE);
            }
        }
        
        DrawText("Pressione ESC para voltar", LARGURA_TELA/2 - 150, ALTURA_TELA - 50, 20, WHITE);
        
        EndDrawing();
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            sair = true;
        }
    }
}

int main() {
    // Inicializa a janela
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Missão no Fundo do Mar");
    SetTargetFPS(60);
    
    // Inicializa o áudio
    InitAudioDevice();
    
    // Inicializa a semente para números aleatórios
    srand(time(NULL));
    
    // Inicia o menu principal
    menuPrincipal();
    
    // Descarrega recursos e fecha a janela
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
} 