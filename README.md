# Missão no Fundo do Mar

Um jogo simples em C onde você controla um submarino para coletar objetos no fundo do mar.

## Estruturas de Dados Utilizadas

1. **Lista Encadeada**: Usada para armazenar os objetos coletáveis no cenário.
2. **Fila**: Implementada para gerenciar a ordem dos jogadores que participam das partidas.
3. **Árvore Binária de Busca**: Utilizada para organizar o ranking de pontuações.
4. **Algoritmo de Ordenação**: Quick Sort para ordenar as pontuações no ranking.

## Como Compilar

Para compilar o jogo, utilize o seguinte comando no terminal:

```
gcc -o missao_no_fundo_do_mar main.c objetos.c ranking.c fila_jogadores.c -Wall
```

## Como Jogar

1. Execute o programa compilado:
   ```
   ./missao_no_fundo_do_mar
   ```

2. No menu principal, você pode:
   - Jogar uma partida
   - Entrar na fila de jogadores
   - Ver o ranking de pontuações
   - Sair do jogo

3. Durante o jogo:
   - Use as teclas W, A, S, D para movimentar o submarino
   - Colete os objetos (representados por 'O') para ganhar pontos
   - O jogo termina após 60 segundos
   - Pressione ESC para encerrar o jogo antecipadamente

## Requisitos

- Sistema operacional Windows (devido ao uso de conio.h e windows.h)
- Compilador GCC

## Estrutura do Projeto

- `main.c`: Arquivo principal com o menu e a lógica do jogo
- `submarino.h`: Definição da estrutura do submarino
- `objetos.h` e `objetos.c`: Implementação da lista de objetos coletáveis
- `ranking.h` e `ranking.c`: Implementação do sistema de ranking com árvore binária
- `fila_jogadores.h` e `fila_jogadores.c`: Implementação da fila de jogadores 