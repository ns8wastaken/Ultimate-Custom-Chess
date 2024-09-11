#include <raylib.h>

#include "constants.hpp"
#include "src/game.cpp"


int main()
{
    InitWindow(Constants::ScreenSize, Constants::ScreenSize, "Ultimate Custom Chess");

    Game game;

    while (!WindowShouldClose()) {
        game.update();
        game.render();
    }

    CloseWindow();
    return 0;
}
