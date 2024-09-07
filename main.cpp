#include <raylib.h>

#include "constants.hpp"
#include "src/engine/engine.cpp"
#include "src/interface/renderer.cpp"


constexpr int ScreenWidth = Constants::SquareWidth * 8;
constexpr int ScreenHeight = Constants::SquareHeight * 8;


int main()
{
    InitWindow(ScreenWidth, ScreenHeight, "Ultimate Custom Chess");

    Engine engine("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    Renderer renderer(ScreenWidth, ScreenHeight);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        Vector2 mousePos = GetMousePosition();
        engine.update(mousePos);

        BeginDrawing();
        renderer.render(deltaTime, engine.c_getFEN());
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
