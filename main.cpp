#include <raylib.h>
#include <iostream>
#include <bitset>

#include "constants.hpp"
#include "src/engine/engine.cpp"
#include "src/interface/renderer.cpp"


int main()
{
    InitWindow(Constants::ScreenSize, Constants::ScreenSize, "Ultimate Custom Chess");

    Engine engine("rnbqkbnr/ppfpppfp/8/8/8/8/PPFPFPPP/RNBQKBNR");
    Renderer renderer;

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        engine.update(mousePos);

        BeginDrawing();
        renderer.render(engine.c_getFEN(), engine.c_getSelectedPiece(), engine.c_getSelectedPieceMoves(), engine.c_getOccupiedSquaresWhite(), engine.c_getOccupiedSquaresBlack());
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
