#include <raylib.h>
#include <iostream>

#include "constants.hpp"
#include "src/engine/engine.cpp"
#include "src/interface/renderer.cpp"


int main()
{
    InitWindow(Constants::ScreenSize, Constants::ScreenSize, "Ultimate Custom Chess");

    Engine engine("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    Renderer renderer;

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        engine.update(mousePos);

        BeginDrawing();
        renderer.render(engine.c_getFEN(), engine.c_getSelectedPiece(), engine.c_getSelectedPieceMoves());
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
