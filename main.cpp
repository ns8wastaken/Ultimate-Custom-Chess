#include <raylib.h>

#include "src/engine/engine.cpp"
#include "src/interface/renderer.cpp"


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting");

    Engine engine;
    Renderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT, engine.getBitboard());

    while (!WindowShouldClose()) {
        // float deltaTime = GetFrameTime();

        BeginDrawing();

        renderer.render();
        // DrawTextureEx(renderer.m_textures['K'], Vector2{ 0, 0 }, 0, 100 / 32, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
