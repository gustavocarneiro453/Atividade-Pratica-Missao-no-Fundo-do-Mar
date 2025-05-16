#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#define NOMINMAX
#define WINDOWS_IGNORE_PACKING_MISMATCH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>  // Adicionado para as funções sin() e cos()
#include "include/raylib.h"  // Path correto para o raylib
#include "submarino.h"
#include "objetos.h"
#include "ranking.h"
#include "fila_jogadores.h"
#include "ia_helper.h"  // Inclui o header da IA

// Funções wrapper para desenho de texto
void DesenharTexto(const char* texto, int x, int y, int tamanho, Color cor) {
    DrawText(texto, x, y, tamanho, cor);
}

void DesenharTextoFormatado(const char* texto, int x, int y, int tamanho, Color cor) {
    DrawText(texto, x, y, tamanho, cor);
}

void DesenharTextoAnimado(const char* texto, Vector2 pos, float tamanho, float espacamento, Color cor) {
    DrawText(texto, (int)pos.x, (int)pos.y, (int)tamanho, cor);
}

void FecharJanela(void) {
    CloseWindow();
}

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define LARGURA_JOGO 70
#define ALTURA_JOGO 20
#define TAMANHO_CELULA 25
#define TEMPO_JOGO 60
#define NUM_TESOUROS 30  // Número de tesouros na tela
#define MAX_BOLHAS 50
#define MAX_EFEITOS 10

// Cores do jogo
#define COR_FUNDO (Color){ 12, 24, 36, 255 }  // Azul escuro para o fundo do mar
#define COR_BORDA (Color){ 0, 162, 232, 255 } // Azul para bordas
#define COR_SUBMARINO (Color){ 255, 255, 0, 255 } // Amarelo para o submarino
#define COR_OBJETO (Color){ 0, 255, 0, 255 }  // Verde para objetos
#define COR_TEXTO (Color){ 255, 255, 255, 255 } // Branco para texto

// Definição adicional de cores (que não estão no Raylib básico)
#define LIGHTBLUE (Color){ 173, 216, 230, 255 }

// Tipos de tesouros e seus valores
#define TIPO_ESMERALDA 0
#define TIPO_RUBI 1
#define TIPO_OURO 2
#define TIPO_DIAMANTE 3
#define TIPO_PEROLA 4
#define NUM_TIPOS_TESOURO 5

// Valores dos tesouros
const int VALORES_TESOUROS[NUM_TIPOS_TESOURO] = {
    20,  // Esmeralda
    15,  // Rubi
    40,  // Ouro
    60,  // Diamante
    30   // Pérola
};

// Cores dos tesouros
const Color CORES_TESOUROS[NUM_TIPOS_TESOURO] = {
    (Color){ 0, 200, 100, 255 },    // Esmeralda: Verde esmeralda
    (Color){ 200, 0, 60, 255 },     // Rubi: Vermelho
    (Color){ 255, 215, 0, 255 },    // Ouro: Dourado
    (Color){ 200, 230, 255, 255 },  // Diamante: Azul claro brilhante
    (Color){ 240, 240, 240, 255 }   // Pérola: Branco perolado
};

// Nomes dos tesouros
const char* NOMES_TESOUROS[NUM_TIPOS_TESOURO] = {
    "Esmeralda",
    "Rubi",
    "Ouro",
    "Diamante",
    "Pérola"
};

// Estruturas para elementos visuais
typedef struct {
    Texture2D textura;
    struct Rectangle origem;  // Adiciona 'struct' antes de Rectangle
} Sprite;

// Estrutura para efeito de coleta
typedef struct {
    float x;
    float y;
    float tempo;
    bool ativo;
    int valor;
    int tipo;
} EfeitoColeta;

// Protótipo da função para exibir o ranking
void exibirRankingGrafico();

// Protótipo da função para exibir a fila
void exibirFilaGrafica(FilaJogadores *fila);

// Protótipo da função para desenhar barra de tempo
void desenharBarraTempo(int tempoRestante, int tempoTotal);

// Protótipo para funções de ranking
void salvarRanking(char nomes[][50], int pontuacoes[], int total);
int carregarRanking(char nomes[][50], int pontuacoes[]);
void adicionarJogadorRanking(char nome[], int pontuacao);

// Protótipo para função de reposicionamento de objetos
void reposicionarObjeto(Objeto *objeto, int max_x, int max_y);

// Protótipos para funções de efeitos visuais
void inicializarEfeitosColeta();
void adicionarEfeitoColetaTesouro(float x, float y, int valor, int tipo);
void atualizarEDesenharEfeitosColeta();

// Função para desenhar submarino mais detalhado
void desenharSubmarinoDetalhado(int x, int y, Color cor) {
    // Corpo principal do submarino
    DrawRectangle(x, y, TAMANHO_CELULA, TAMANHO_CELULA, cor);
    
    // Escotilha
    DrawCircle(x + TAMANHO_CELULA/2, y + TAMANHO_CELULA/2, TAMANHO_CELULA/4, DARKBLUE);
    DrawCircle(x + TAMANHO_CELULA/2, y + TAMANHO_CELULA/2, TAMANHO_CELULA/6, SKYBLUE);
    
    // Periscópio
    DrawRectangle(x + TAMANHO_CELULA/2 - 2, y - 5, 4, 5, DARKGRAY);
    
    // Propulsor
    DrawRectangle(x - 5, y + TAMANHO_CELULA/2 - 3, 5, 6, GRAY);
}

// Função para desenhar tesouro mais detalhado
void desenharTesouroDetalhado(int x, int y, int tipo) {
    Color corTesouro = CORES_TESOUROS[tipo];
    
    switch (tipo) {
        case TIPO_ESMERALDA: 
            // Esmeralda: cristal octogonal
            DrawRectangle(x + 5, y + 5, TAMANHO_CELULA - 10, TAMANHO_CELULA - 10, corTesouro);
            DrawLine(x + 5, y + 8, x + TAMANHO_CELULA - 5, y + 8, DARKGREEN);
            DrawLine(x + 5, y + 12, x + TAMANHO_CELULA - 5, y + 12, DARKGREEN);
            DrawLine(x + 5, y + 16, x + TAMANHO_CELULA - 5, y + 16, DARKGREEN);
            break;
            
        case TIPO_RUBI:
            // Rubi: gema triangular
            DrawTriangle(
                (Vector2){x + TAMANHO_CELULA/2, y + 3},
                (Vector2){x + 3, y + TAMANHO_CELULA - 3},
                (Vector2){x + TAMANHO_CELULA - 3, y + TAMANHO_CELULA - 3},
                corTesouro
            );
            DrawTriangleLines(
                (Vector2){x + TAMANHO_CELULA/2, y + 3},
                (Vector2){x + 3, y + TAMANHO_CELULA - 3},
                (Vector2){x + TAMANHO_CELULA - 3, y + TAMANHO_CELULA - 3},
                MAROON
            );
            break;
            
        case TIPO_OURO:
            // Ouro: barras de ouro empilhadas
            DrawRectangle(x + 3, y + 10, TAMANHO_CELULA - 6, 5, GOLD);
            DrawRectangle(x + 3, y + 15, TAMANHO_CELULA - 6, 5, GOLD);
            DrawRectangle(x + 8, y + 5, TAMANHO_CELULA - 16, 5, GOLD);
            DrawRectangleLines(x + 3, y + 10, TAMANHO_CELULA - 6, 5, BROWN);
            DrawRectangleLines(x + 3, y + 15, TAMANHO_CELULA - 6, 5, BROWN);
            DrawRectangleLines(x + 8, y + 5, TAMANHO_CELULA - 16, 5, BROWN);
            break;
            
        case TIPO_DIAMANTE:
            // Diamante: pedra brilhante
            DrawRectangle(x + 5, y + 5, TAMANHO_CELULA - 10, TAMANHO_CELULA - 10, corTesouro);
            DrawLine(x + TAMANHO_CELULA/2, y + 3, x + 3, y + TAMANHO_CELULA/2, WHITE);
            DrawLine(x + TAMANHO_CELULA/2, y + 3, x + TAMANHO_CELULA - 3, y + TAMANHO_CELULA/2, WHITE);
            DrawLine(x + 3, y + TAMANHO_CELULA/2, x + TAMANHO_CELULA/2, y + TAMANHO_CELULA - 3, WHITE);
            DrawLine(x + TAMANHO_CELULA - 3, y + TAMANHO_CELULA/2, x + TAMANHO_CELULA/2, y + TAMANHO_CELULA - 3, WHITE);
            break;
            
        case TIPO_PEROLA:
            // Pérola: esfera brilhante
            DrawCircle(x + TAMANHO_CELULA/2, y + TAMANHO_CELULA/2, TAMANHO_CELULA/2 - 5, corTesouro);
            DrawCircle(x + TAMANHO_CELULA/2 - 2, y + TAMANHO_CELULA/2 - 2, 2, WHITE);
            break;
    }
}

// Função para criar uma bolha
typedef struct {
    float x;
    float y;
    float velocidade;
    float raio;
    Color cor;
    bool ativa;
} Bolha;

Bolha bolhas[MAX_BOLHAS];
EfeitoColeta efeitosColeta[MAX_EFEITOS];

// Inicializa o sistema de bolhas
void inicializarBolhas() {
    for (int i = 0; i < MAX_BOLHAS; i++) {
        bolhas[i].ativa = false;
    }
}

// Cria uma nova bolha
void criarBolha(int x, int y) {
    for (int i = 0; i < MAX_BOLHAS; i++) {
        if (!bolhas[i].ativa) {
            bolhas[i].x = x + (float)GetRandomValue(0, TAMANHO_CELULA);
            bolhas[i].y = y + (float)TAMANHO_CELULA;
            bolhas[i].velocidade = (float)GetRandomValue(1, 3) / 2.0f;
            bolhas[i].raio = (float)GetRandomValue(1, 3);
            bolhas[i].cor = (Color){ 200, 200, 255, GetRandomValue(100, 200) };
            bolhas[i].ativa = true;
            break;
        }
    }
}

// Atualiza e desenha as bolhas
void atualizarEDesenharBolhas() {
    for (int i = 0; i < MAX_BOLHAS; i++) {
        if (bolhas[i].ativa) {
            // Atualiza posição
            bolhas[i].y -= bolhas[i].velocidade;
            
            // Verifica se saiu da tela
            if (bolhas[i].y < 0) {
                bolhas[i].ativa = false;
                continue;
            }
            
            // Desenha a bolha
            DrawCircleV((Vector2){ bolhas[i].x, bolhas[i].y }, bolhas[i].raio, bolhas[i].cor);
        }
    }
}

// Função para carregar texturas
void carregarTexturas(Sprite *submarinoSprite, Sprite *objetoSprite) {
    // Submarino (retângulo amarelo)
    Image imagemSubmarino = GenImageColor(TAMANHO_CELULA, TAMANHO_CELULA, YELLOW);
    submarinoSprite->textura = LoadTextureFromImage(imagemSubmarino);
    submarinoSprite->origem = (struct Rectangle){.x = 0.0f, .y = 0.0f, .width = (float)TAMANHO_CELULA, .height = (float)TAMANHO_CELULA};
    UnloadImage(imagemSubmarino);
    
    // Objeto (círculo verde)
    Image imagemObjeto = GenImageColor(TAMANHO_CELULA, TAMANHO_CELULA, GREEN);
    objetoSprite->textura = LoadTextureFromImage(imagemObjeto);
    objetoSprite->origem = (struct Rectangle){.x = 0.0f, .y = 0.0f, .width = (float)TAMANHO_CELULA, .height = (float)TAMANHO_CELULA};
    UnloadImage(imagemObjeto);
}

// Função para desenhar o cenário
void desenharCenario(Submarino *submarino, ListaObjetos *objetos, Sprite submarinoSprite, Sprite objetoSprite) {
    int i, j;
    int offsetX = (LARGURA_TELA - LARGURA_JOGO * TAMANHO_CELULA) / 2;
    int offsetY = (ALTURA_TELA - ALTURA_JOGO * TAMANHO_CELULA) / 2;
    
    // Desenha o fundo
    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, COR_FUNDO);
    
    // Desenha fundo do oceano (areia)
    DrawRectangle(offsetX, offsetY + ALTURA_JOGO * TAMANHO_CELULA - TAMANHO_CELULA, 
                 LARGURA_JOGO * TAMANHO_CELULA, TAMANHO_CELULA, BEIGE);
    
    // Desenha plantas marinhas aleatórias
    for (i = 0; i < 10; i++) {
        int plantaX = offsetX + GetRandomValue(2, LARGURA_JOGO-3) * TAMANHO_CELULA;
        int plantaY = offsetY + (ALTURA_JOGO-1) * TAMANHO_CELULA;
        DrawRectangle(plantaX, plantaY - GetRandomValue(5, 15), 2, GetRandomValue(15, 30), DARKGREEN);
    }
    
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
    
    // Cria bolhas ocasionalmente a partir do submarino
    if (GetRandomValue(0, 30) == 0) {
        criarBolha(offsetX + submarino->x * TAMANHO_CELULA, 
                  offsetY + submarino->y * TAMANHO_CELULA);
    }
    
    // Atualiza e desenha bolhas
    atualizarEDesenharBolhas();
    
    // Desenha o submarino detalhado
    desenharSubmarinoDetalhado(
        offsetX + submarino->x * TAMANHO_CELULA,
        offsetY + submarino->y * TAMANHO_CELULA,
        COR_SUBMARINO
    );
    
    // Desenha os objetos
    Objeto *atual = objetos->inicio;
    while (atual != NULL) {
        if (atual->visivel) {
            desenharTesouroDetalhado(
                offsetX + atual->x * TAMANHO_CELULA,
                offsetY + atual->y * TAMANHO_CELULA,
                atual->tipo
            );
        }
        atual = atual->prox;
    }
    
    // Desenha as informações do jogo
    DesenharTexto("Missão no Fundo do Mar", 20, 20, 20, COR_TEXTO);
    
    // Desenha os efeitos de coleta
    atualizarEDesenharEfeitosColeta();
}

// Função para desenhar barra de tempo
void desenharBarraTempo(int tempoRestante, int tempoTotal) {
    // Calcula a porcentagem do tempo restante
    float porcentagem = (float)tempoRestante / (float)tempoTotal;
    
    // Desenha o contorno da barra
    DrawRectangle(LARGURA_TELA - 220, 80, 200, 20, GRAY);
    
    // Desenha o preenchimento da barra
    Color corTempo = GREEN;
    if (porcentagem < 0.5) corTempo = YELLOW;
    if (porcentagem < 0.25) corTempo = RED;
    
    DrawRectangle(LARGURA_TELA - 220, 80, (int)(200 * porcentagem), 20, corTempo);
    
    // Texto do tempo
    DesenharTextoFormatado(TextFormat("Tempo: %d s", tempoRestante), LARGURA_TELA - 220, 105, 16, WHITE);
}

// Função para reposicionar um objeto em uma nova localização aleatória
void reposicionarObjeto(Objeto *objeto, int max_x, int max_y) {
    // Gera posições aleatórias (evitando as bordas)
    objeto->x = (rand() % (max_x - 2)) + 1;
    objeto->y = (rand() % (max_y - 2)) + 1;
    objeto->visivel = 1;  // Garante que o objeto está visível
    
    // Atribui um tipo aleatório de tesouro
    objeto->tipo = rand() % NUM_TIPOS_TESOURO;
    
    // Define o valor com base no tipo
    objeto->valor = VALORES_TESOUROS[objeto->tipo];
}

// Verifica colisão entre submarino e objetos
void verificarColisao(Submarino *submarino, ListaObjetos *objetos, int *pontos) {
    Objeto *atual = objetos->inicio;
    
    while (atual != NULL) {
        if (atual->visivel && atual->x == submarino->x && atual->y == submarino->y) {
            *pontos += atual->valor;
            
            // Adiciona efeito visual na coleta
            int offsetX = (LARGURA_TELA - LARGURA_JOGO * TAMANHO_CELULA) / 2;
            int offsetY = (ALTURA_TELA - ALTURA_JOGO * TAMANHO_CELULA) / 2;
            adicionarEfeitoColetaTesouro(
                offsetX + atual->x * TAMANHO_CELULA + TAMANHO_CELULA/2,
                offsetY + atual->y * TAMANHO_CELULA,
                atual->valor,
                atual->tipo
            );
            
            // Reposiciona o objeto em vez de escondê-lo
            reposicionarObjeto(atual, LARGURA_JOGO, ALTURA_JOGO);
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
    int tempo_restante = TEMPO_JOGO;
    Sprite submarinoSprite, objetoSprite;
    int tesouros_coletados = 0;  // Novo contador
    char dica_atual[1024] = "";  // Buffer para a dica da IA
    
    // Inicializa o sistema de IA
    inicializar_ia();
    
    // Inicializa o submarino
    submarino.x = LARGURA_JOGO / 2;
    submarino.y = ALTURA_JOGO / 2;
    
    // Inicializa a lista de objetos
    inicializarListaObjetos(&objetos);
    
    // Adiciona objetos iniciais
    int i;
    for (i = 0; i < NUM_TESOUROS; i++) {
        adicionarObjetoAleatorio(&objetos, LARGURA_JOGO, ALTURA_JOGO);
    }
    
    // Carrega as texturas
    carregarTexturas(&submarinoSprite, &objetoSprite);
    
    // Inicializa o sistema de bolhas
    inicializarBolhas();
    
    // Inicializa os efeitos de coleta
    inicializarEfeitosColeta();
    
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
        
        // Verifica colisões e atualiza contadores
        int pontos_anteriores = pontos;
        verificarColisao(&submarino, &objetos, &pontos);
        if (pontos > pontos_anteriores) {
            tesouros_coletados++;
        }
        
        // Obtém dicas da IA periodicamente
        char* nova_dica = obter_dica_ia(pontos, tempo_restante, tesouros_coletados);
        if (strlen(nova_dica) > 0) {
            strcpy(dica_atual, nova_dica);
        }
        
        // Desenha o jogo
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Desenha o cenário do jogo
        desenharCenario(&submarino, &objetos, submarinoSprite, objetoSprite);
        
        // Desenha as informações do jogo
        DesenharTextoFormatado(TextFormat("Pontuação: %d", pontos), LARGURA_TELA - 200, 50, 20, COR_TEXTO);
        DesenharTextoFormatado(TextFormat("Jogador: %s", nome), 20, 50, 20, COR_TEXTO);
        
        // Desenha a barra de tempo
        desenharBarraTempo(tempo_restante, TEMPO_JOGO);
        
        // Desenha a dica da IA apenas se houver uma dica
        if (strlen(dica_atual) > 0) {
            int largura_texto = MeasureText(dica_atual, 20);
            int x = (LARGURA_TELA - largura_texto) / 2;
            DrawRectangle(x - 10, ALTURA_TELA - 40, largura_texto + 20, 30, (Color){0, 0, 0, 200});
            DesenharTexto(dica_atual, x, ALTURA_TELA - 35, 20, YELLOW);
        }
        
        EndDrawing();
    }
    
    // Descarrega as texturas
    UnloadTexture(submarinoSprite.textura);
    UnloadTexture(objetoSprite.textura);
    
    // Fim do jogo - Tela de resultados
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        DesenharTexto("Fim de jogo!", LARGURA_TELA/2 - 100, ALTURA_TELA/2 - 50, 30, WHITE);
        DesenharTextoFormatado(TextFormat("Jogador: %s", nome), LARGURA_TELA/2 - 100, ALTURA_TELA/2, 20, WHITE);
        DesenharTextoFormatado(TextFormat("Pontuação final: %d", pontos), LARGURA_TELA/2 - 100, ALTURA_TELA/2 + 30, 20, WHITE);
        DesenharTexto("Pressione ESC para voltar ao menu", LARGURA_TELA/2 - 180, ALTURA_TELA/2 + 80, 20, WHITE);
        
        EndDrawing();
        
        if (IsKeyPressed(KEY_ESCAPE)) break;
    }
    
    // Adiciona a pontuação ao ranking
    adicionarJogadorRanking(nome, pontos);
    
    // Libera a memória da lista de objetos
    liberarListaObjetos(&objetos);
    
    // Finaliza o sistema de IA
    finalizar_ia();
}

// Função para exibir a fila atual de jogadores
void exibirFilaGrafica(FilaJogadores *fila) {
    bool sair = false;
    int totalJogadores = tamanhoFila(fila);
    
    // Array temporário para armazenar os nomes
    char nomes[20][50];  // Máximo de 20 jogadores na visualização
    int contador = 0;
    
    NoJogador *atual = fila->inicio;
    while (atual != NULL && contador < 20) {
        strcpy(nomes[contador], atual->nome);
        contador++;
        atual = atual->prox;
    }
    
    // Loop de exibição da fila
    while (!sair && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(COR_FUNDO);
        
        DesenharTexto("FILA DE JOGADORES", LARGURA_TELA/2 - 150, 50, 30, WHITE);
        
        if (totalJogadores == 0) {
            DesenharTexto("Não há jogadores na fila!", LARGURA_TELA/2 - 150, ALTURA_TELA/2, 20, WHITE);
        } else {
            for (int i = 0; i < contador; i++) {
                DesenharTextoFormatado(TextFormat("%d. %s", i+1, nomes[i]), LARGURA_TELA/2 - 150, 120 + i*30, 20, WHITE);
                
                // Desenha indicação visual para o próximo a jogar
                if (i == 0) {
                    DesenharTexto("(Próximo a jogar)", LARGURA_TELA/2 + 120, 120, 20, YELLOW);
                }
            }
        }
        
        DesenharTexto("Pressione ESC para voltar", LARGURA_TELA/2 - 150, ALTURA_TELA - 50, 20, WHITE);
        
        EndDrawing();
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            sair = true;
        }
    }
}

// Função para exibir o ranking com interface gráfica
void exibirRankingGrafico() {
    // Obtém o ranking ordenado
    char nomes[10][50]; // Assumindo máximo de 10 jogadores no ranking
    int pontuacoes[10];
    int totalJogadores = carregarRanking(nomes, pontuacoes);
    
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
    
    // Salva o ranking ordenado
    salvarRanking(nomes, pontuacoes, totalJogadores);
    
    // Loop de exibição do ranking
    bool sair = false;
    float tempoAnimacao = 0;
    
    while (!sair && !WindowShouldClose()) {
        tempoAnimacao += GetFrameTime();
        BeginDrawing();
        ClearBackground(COR_FUNDO);
        
        // Adiciona movimento às ondas do mar no fundo
        for (int i = 0; i < LARGURA_TELA; i += 50) {
            float altura = 10 * sin(tempoAnimacao * 2 + i * 0.1);
            DrawRectangle(i, ALTURA_TELA - 50 + (int)altura, 50, 50, DARKBLUE);
        }
        
        // Título com efeito
        Color corTitulo = WHITE;
        if (sin(tempoAnimacao * 2) > 0) {
            corTitulo = GOLD;
        }
        DesenharTexto("RANKING DE JOGADORES", LARGURA_TELA/2 - 180, 50, 30, corTitulo);
        
        // Desenha decoração temática submarina
        DrawCircle(LARGURA_TELA - 50, 50, 20 + sin(tempoAnimacao * 3) * 5, LIGHTBLUE);  // Bolha
        
        // Desenha um pequeno submarino animado
        float subX = 50 + sin(tempoAnimacao) * 30;
        float subY = 50 + cos(tempoAnimacao * 2) * 10;
        desenharSubmarinoDetalhado(subX, subY, YELLOW);
        
        // Desenha as linhas do ranking
        if (totalJogadores == 0) {
            DesenharTexto("Nenhum jogador no ranking!", LARGURA_TELA/2 - 150, ALTURA_TELA/2, 20, WHITE);
        } else {
            DrawRectangle(LARGURA_TELA/2 - 270, 110, 540, 40, (Color){0, 0, 80, 150});
            DesenharTexto("Posição", LARGURA_TELA/2 - 250, 120, 20, YELLOW);
            DesenharTexto("Nome", LARGURA_TELA/2 - 150, 120, 20, YELLOW);
            DesenharTexto("Pontuação", LARGURA_TELA/2 + 100, 120, 20, YELLOW);
            
            for (int i = 0; i < totalJogadores; i++) {
                // Cria uma visualização alternando cores para melhor legibilidade
                if (i % 2 == 0) {
                    DrawRectangle(LARGURA_TELA/2 - 270, 160 + i*30, 540, 25, (Color){0, 0, 50, 100});
                }
                
                // Destaque para os três primeiros lugares
                Color corPosicao = WHITE;
                if (i == 0) corPosicao = GOLD;
                else if (i == 1) corPosicao = LIGHTGRAY;
                else if (i == 2) corPosicao = BROWN;
                
                DesenharTextoFormatado(TextFormat("%d.", i+1), LARGURA_TELA/2 - 250, 160 + i*30, 20, corPosicao);
                DesenharTexto(nomes[i], LARGURA_TELA/2 - 150, 160 + i*30, 20, WHITE);
                DesenharTextoFormatado(TextFormat("%d", pontuacoes[i]), LARGURA_TELA/2 + 100, 160 + i*30, 20, WHITE);
                
                // Anima o número da pontuação para os primeiros lugares
                if (i < 3) {
                    float scale = 1.0f + 0.1f * sin(tempoAnimacao * 3);
                    Vector2 textSize = MeasureTextEx(GetFontDefault(), TextFormat("%d", pontuacoes[i]), 20, 1);
                    Vector2 position = {LARGURA_TELA/2 + 100 + textSize.x/2, 160 + i*30 + textSize.y/2};
                    DesenharTextoAnimado(TextFormat("%d", pontuacoes[i]), 
                              (Vector2){position.x - textSize.x*scale/2, position.y - textSize.y*scale/2}, 
                              20*scale, 1, corPosicao);
                }
            }
        }
        
        DesenharTexto("Pressione ESC para voltar", LARGURA_TELA/2 - 150, ALTURA_TELA - 50, 20, WHITE);
        
        EndDrawing();
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            sair = true;
        }
    }
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
        DesenharTexto("MISSÃO NO FUNDO DO MAR", LARGURA_TELA/2 - 200, 100, 30, WHITE);
        
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
                DesenharTexto(opcoes[i], LARGURA_TELA/2 - 100, 200 + i*40, 20, YELLOW);
            } else {
                DesenharTexto(opcoes[i], LARGURA_TELA/2 - 100, 200 + i*40, 20, WHITE);
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
            DesenharTexto("Digite seu nome:", LARGURA_TELA/2 - 180, ALTURA_TELA/2 + 20, 20, WHITE);
            DesenharTexto(nome, LARGURA_TELA/2 - 180, ALTURA_TELA/2 + 50, 20, WHITE);
            
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
                    FecharJanela();
                    break;
            }
        }
        
        // Desenha o número de jogadores na fila
        int numJogadoresFila = tamanhoFila(&fila);
        if (numJogadoresFila > 0) {
            DesenharTextoFormatado(TextFormat("Jogadores na fila: %d", numJogadoresFila), 20, ALTURA_TELA - 40, 20, YELLOW);
            
            // Botão para visualizar a fila
            DrawRectangle(LARGURA_TELA - 220, ALTURA_TELA - 50, 200, 30, DARKBLUE);
            DesenharTexto("Ver Fila (F)", LARGURA_TELA - 200, ALTURA_TELA - 45, 20, WHITE);
            
            // Tecla para visualizar a fila
            if (IsKeyPressed(KEY_F)) {
                exibirFilaGrafica(&fila);
            }
        }
        
        EndDrawing();
    }
    
    // Libera a memória da fila
    liberarFila(&fila);
    
    // Libera a memória do ranking
    liberarRanking();
}

// Função para salvar o ranking em arquivo
void salvarRanking(char nomes[][50], int pontuacoes[], int total) {
    FILE *arquivo = fopen("ranking.dat", "wb");
    if (arquivo == NULL) {
        return;
    }
    
    for (int i = 0; i < total; i++) {
        Ranking jogador;
        strcpy(jogador.nome, nomes[i]);
        jogador.pontuacao = pontuacoes[i];
        jogador.esq = NULL;
        jogador.dir = NULL;
        fwrite(&jogador, sizeof(Ranking), 1, arquivo);
    }
    
    fclose(arquivo);
}

// Função para carregar ranking do arquivo
int carregarRanking(char nomes[][50], int pontuacoes[]) {
    int totalJogadores = 0;
    
    // Primeiro, tenta carregar do ranking.dat (formato binário)
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
    
    // Se não encontrou ou está vazio, cria um arquivo novo
    if (totalJogadores == 0) {
        arquivo = fopen("ranking.txt", "w");
        if (arquivo != NULL) {
            fprintf(arquivo, "# Ranking - Missão no Fundo do Mar\n");
            fprintf(arquivo, "# Formato: Nome,Pontuação\n");
            fclose(arquivo);
        }
    }
    
    return totalJogadores;
}

// Função para adicionar novo jogador ao ranking
void adicionarJogadorRanking(char nome[], int pontuacao) {
    // Carrega o ranking atual
    char nomes[10][50];
    int pontuacoes[10];
    int totalJogadores = carregarRanking(nomes, pontuacoes);
    
    // Verifica se o jogador já existe no ranking
    int posicaoExistente = -1;
    for (int i = 0; i < totalJogadores; i++) {
        if (strcmp(nomes[i], nome) == 0) {
            posicaoExistente = i;
            break;
        }
    }
    
    if (posicaoExistente >= 0) {
        // Atualiza a pontuação se for maior que a existente
        if (pontuacao > pontuacoes[posicaoExistente]) {
            pontuacoes[posicaoExistente] = pontuacao;
        }
    } else {
        // Adiciona novo jogador se houver espaço
        if (totalJogadores < 10) {
            strcpy(nomes[totalJogadores], nome);
            pontuacoes[totalJogadores] = pontuacao;
            totalJogadores++;
        } else {
            // Verifica se a pontuação é maior que a menor do ranking
            int menorPontuacao = pontuacoes[0];
            int posicaoMenor = 0;
            
            for (int i = 1; i < totalJogadores; i++) {
                if (pontuacoes[i] < menorPontuacao) {
                    menorPontuacao = pontuacoes[i];
                    posicaoMenor = i;
                }
            }
            
            if (pontuacao > menorPontuacao) {
                strcpy(nomes[posicaoMenor], nome);
                pontuacoes[posicaoMenor] = pontuacao;
            }
        }
    }
    
    // Ordena e salva o ranking
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
    
    // Salva o ranking
    salvarRanking(nomes, pontuacoes, totalJogadores);
    
    // Também salva em formato texto para depuração
    FILE *arquivoTexto = fopen("ranking.txt", "w");
    if (arquivoTexto != NULL) {
        fprintf(arquivoTexto, "# Ranking - Missão no Fundo do Mar\n");
        fprintf(arquivoTexto, "# Formato: Nome,Pontuação\n");
        
        for (int i = 0; i < totalJogadores; i++) {
            fprintf(arquivoTexto, "%s,%d\n", nomes[i], pontuacoes[i]);
        }
        
        fclose(arquivoTexto);
    }
}

// Inicializa os efeitos de coleta
void inicializarEfeitosColeta() {
    for (int i = 0; i < MAX_EFEITOS; i++) {
        efeitosColeta[i].ativo = false;
    }
}

// Adiciona um novo efeito de coleta
void adicionarEfeitoColeta(float x, float y, int valor) {
    for (int i = 0; i < MAX_EFEITOS; i++) {
        if (!efeitosColeta[i].ativo) {
            efeitosColeta[i].x = x;
            efeitosColeta[i].y = y;
            efeitosColeta[i].tempo = 0;
            efeitosColeta[i].ativo = true;
            efeitosColeta[i].valor = valor;
            break;
        }
    }
}

// Adiciona um efeito de texto quando um tesouro é coletado
void adicionarEfeitoColetaTesouro(float x, float y, int valor, int tipo) {
    for (int i = 0; i < MAX_EFEITOS; i++) {
        if (!efeitosColeta[i].ativo) {
            efeitosColeta[i].x = x;
            efeitosColeta[i].y = y;
            efeitosColeta[i].tempo = 0;
            efeitosColeta[i].ativo = true;
            efeitosColeta[i].valor = valor;
            efeitosColeta[i].tipo = tipo;
            break;
        }
    }
}

// Atualiza e desenha os efeitos de coleta
void atualizarEDesenharEfeitosColeta() {
    for (int i = 0; i < MAX_EFEITOS; i++) {
        if (efeitosColeta[i].ativo) {
            // Atualiza o tempo
            efeitosColeta[i].tempo += GetFrameTime() * 2;
            efeitosColeta[i].y -= 1.0f;  // Move para cima
            
            // Verifica se o efeito expirou
            if (efeitosColeta[i].tempo >= 1.0f) {
                efeitosColeta[i].ativo = false;
                continue;
            }
            
            // Calcula a transparência (fade out)
            float alpha = 1.0f - efeitosColeta[i].tempo;
            
            // Escolhe a cor baseada no tipo de tesouro
            Color corTexto = CORES_TESOUROS[efeitosColeta[i].tipo];
            corTexto.a = (unsigned char)(255 * alpha);
            
            // Desenha o nome do tesouro e o valor
            DesenharTextoAnimado(
                NOMES_TESOUROS[efeitosColeta[i].tipo],
                (Vector2){efeitosColeta[i].x - 30, efeitosColeta[i].y - 20},
                16 + (1.0f - alpha) * 6,
                1,
                corTexto
            );
            
            // Desenha o texto com o valor
            DesenharTextoAnimado(
                TextFormat("+%d", efeitosColeta[i].valor),
                (Vector2){efeitosColeta[i].x, efeitosColeta[i].y},
                20 + (1.0f - alpha) * 10,
                1,
                corTexto
            );
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