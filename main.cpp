#include <raylib.h>
#include <iostream>

#include "src/engine/engine.cpp"
#include "src/interface/renderer.cpp"


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ultimate Custom Chess");

    Engine engine("r3k2r/pp1n2pp/2p2q2/b2p1n2/BP1Pp3/P1N2P2/2PB2PP/R2Q1RK1");
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);

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
