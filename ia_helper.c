#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "ia_helper.h"

#define GEMINI_API_KEY "AIzaSyBBp9kF6r-kIe0ACaqNzdUQm-j606hjF9A"
#define GEMINI_API_URL "https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent"

// Estrutura para armazenar a resposta da API
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback para receber os dados da resposta HTTP
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        printf("Sem memória suficiente (realloc retornou NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Variável global para a sessão CURL
static CURL *curl = NULL;

// Inicializa o sistema de IA
void inicializar_ia(void) {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
}

// Obtém uma dica da IA com base no estado atual do jogo
char* obter_dica_ia(int pontos, int tempo_restante, int tesouros_coletados) {
    if (!curl) return "Sistema de IA não inicializado";

    static char dica[1024];
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    // Prepara o payload JSON
    char payload[2048];
    snprintf(payload, sizeof(payload),
        "{"
        "\"contents\": [{"
        "\"parts\":[{\"text\": \"Você é um assistente de jogo submarino. "
        "O jogador tem %d pontos, %d segundos restantes e coletou %d tesouros. "
        "Dê uma dica curta e motivadora em português (máximo 100 caracteres) "
        "para ajudá-lo a melhorar sua pontuação.\"}]"
        "}]"
        "}", pontos, tempo_restante, tesouros_coletados);

    // Configura a requisição CURL
    char url[512];
    snprintf(url, sizeof(url), "%s?key=%s", GEMINI_API_URL, GEMINI_API_KEY);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // Realiza a requisição
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        snprintf(dica, sizeof(dica), "Erro ao obter dica: %s", curl_easy_strerror(res));
    } else {
        // Processa a resposta JSON para extrair apenas o texto da dica
        // Simplificado para exemplo - você pode usar uma biblioteca JSON adequada
        char *start = strstr(chunk.memory, "\"text\": \"");
        if (start) {
            start += 9; // Pula "\"text\": \""
            char *end = strchr(start, '\"');
            if (end) {
                size_t len = end - start;
                if (len > sizeof(dica) - 1) len = sizeof(dica) - 1;
                strncpy(dica, start, len);
                dica[len] = '\0';
            } else {
                strcpy(dica, "Explore o fundo do mar com sabedoria!");
            }
        } else {
            strcpy(dica, "Continue explorando!");
        }
    }

    // Limpa a memória
    curl_slist_free_all(headers);
    free(chunk.memory);

    return dica;
}

// Finaliza o sistema de IA
void finalizar_ia(void) {
    if (curl) {
        curl_easy_cleanup(curl);
        curl = NULL;
    }
    curl_global_cleanup();
} 