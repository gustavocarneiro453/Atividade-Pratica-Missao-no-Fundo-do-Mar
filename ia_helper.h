#ifndef IA_HELPER_H
#define IA_HELPER_H

#include <curl/curl.h>

// Estrutura para armazenar a resposta da API
typedef struct {
    char *buffer;
    size_t size;
} MemoryStruct;

// Inicializa o sistema de IA
void inicializar_ia(void);

// Obtém uma dica da IA com base no estado atual do jogo
char* obter_dica_ia(int pontos, int tempo_restante, int tesouros_coletados);

// Finaliza o sistema de IA
void finalizar_ia(void);

#endif 