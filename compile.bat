@echo off
echo Compilando o jogo com Raylib e Gemini API...
gcc raylib_game.c ia_helper.c objetos.c ranking.c fila_jogadores.c -o jogo.exe -I./include -I./curl-8.13.0/include -L./lib -L./curl-8.13.0/lib -lcurl -lraylib -lwinmm -lgdi32 -lopengl32
echo.
if %errorlevel% == 0 (
    echo Compilacao concluida com sucesso!
    echo Executando o jogo...
    jogo.exe
) else (
    echo Erro na compilacao!
)
pause 