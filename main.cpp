#include <raylib.h>

#include "src/engine/engine.cpp"
#include "src/interface/renderer.cpp"


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ultimate Custom Chess");

    Engine engine;
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, engine.c_getBitboard());

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        Vector2 mousePos = GetMousePosition();
        engine.update(mousePos);

        BeginDrawing();
        renderer.render(deltaTime);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
