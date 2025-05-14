# Missão no Fundo do Mar - Versão Gráfica com Raylib

Este arquivo contém instruções para compilar e executar a versão gráfica do jogo "Missão no Fundo do Mar" usando a biblioteca Raylib.

## Estrutura do Projeto

- `raylib_game.c` - Implementação do jogo usando a biblioteca Raylib
- `objetos.c/h` - Gerenciamento dos objetos coletáveis
- `submarino.h` - Definição da estrutura do submarino
- `ranking.c/h` - Sistema de ranking de jogadores
- `fila_jogadores.c/h` - Implementação da fila de jogadores
- `include/` - Diretório contendo os cabeçalhos do Raylib
- `lib/` - Diretório contendo as bibliotecas do Raylib
- `compile.bat` - Script para compilar o jogo no Windows

## Como Compilar

No Windows, basta executar o arquivo `compile.bat`. Isso compilará o jogo e gerará o arquivo executável `missao_raylib.exe`.

A linha de compilação é:
```
gcc raylib_game.c objetos.c fila_jogadores.c ranking.c -o missao_raylib.exe -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

## Como Jogar

Após a compilação, o jogo será executado automaticamente. O jogo possui um menu com as seguintes opções:

1. **Jogar**: Inicia o jogo diretamente (se não houver jogadores na fila) ou com o próximo jogador da fila.
2. **Entrar na fila de jogadores**: Adiciona um jogador à fila de espera.
3. **Exibir ranking**: Mostra o ranking dos jogadores com maior pontuação.
4. **Sair**: Encerra o jogo.

### Controles

- Use as setas do teclado ou as teclas W, A, S, D para movimentar o submarino.
- Colete os objetos verdes para ganhar pontos.
- Pressione ESC para sair do jogo a qualquer momento.

## Diferenças para a Versão Original

A versão gráfica com Raylib difere da versão de console nos seguintes aspectos:

1. Interface gráfica colorida em vez de caracteres ASCII.
2. Controles mais responsivos com detecção de teclas por frame.
3. Menu interativo com opções selecionáveis.
4. Visualização gráfica do ranking de jogadores.
5. Tempo de jogo estendido para 60 segundos (configurável através da constante `TEMPO_JOGO`).

## Requisitos

- Windows (para usar o script de compilação fornecido)
- GCC (Compilador C)
- As bibliotecas do Raylib (já incluídas no projeto)

## Observações

Este jogo é uma versão gráfica do jogo "Missão no Fundo do Mar" desenvolvido originalmente como projeto de Estrutura de Dados. A conversão para Raylib mantém a mesma lógica e estrutura de dados, mas adiciona uma interface gráfica mais amigável. 