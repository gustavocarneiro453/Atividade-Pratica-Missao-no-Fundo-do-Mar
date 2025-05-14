@echo off
echo Compilando o jogo com Raylib...
gcc raylib_game.c objetos.c fila_jogadores.c ranking.c -o missao_raylib.exe -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm
echo.
if %errorlevel% == 0 (
    echo Compilacao concluida com sucesso!
    echo Executando o jogo...
    missao_raylib.exe
) else (
    echo Erro na compilacao!
)
pause 