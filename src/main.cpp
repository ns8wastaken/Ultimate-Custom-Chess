#include <raylib.h>
#include <iostream>
#include <bitset>

#include "constants.hpp"
#include "game.cpp"


int main()
{
    InitWindow(Constants::ScreenSize, Constants::ScreenSize, "Ultimate Custom Chess");

    Image icon = LoadImage("assets/pieces/pawn/pawn_white.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    Game game;

    while (!WindowShouldClose()) {
        game.update();
        game.render();
    }

    CloseWindow();
    return 0;
}
