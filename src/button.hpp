#pragma once
#include <raylib.h>

#include "constants.hpp"


class Button
{
public:
    Button(Vector2 pos, const char* imagePath);

    bool update(const Vector2& mousePos);
    void render();

private:
    Texture m_texture;
    Vector2 m_pos;
};
