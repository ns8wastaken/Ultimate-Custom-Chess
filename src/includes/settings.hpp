#pragma once
#include <raylib.h>


namespace Settings
{
    struct BoardColors
    {
        Color dark;
        Color light;
    };

    BoardColors BackgroundColors[] = {
        BoardColors{Color{118, 150, 86, 255},  Color{238, 238, 210, 255}},
        BoardColors{Color{174, 200, 161, 255}, Color{255, 249, 233, 255}}
    };
}
