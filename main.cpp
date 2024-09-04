#include <raylib.h>


#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 800


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting");

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
