#pragma once
#include "engine/engine.hpp"
#include "interface/board_renderer.hpp"

#include "button.cpp"


class Game
{
public:
    Game();

    void update();
    void render();

private:
    char m_mode = 0;
    Engine m_engine;
    Renderer m_renderer;

    Button m_play;

    Button m_vsBot;
    Button m_vsFriend;

    Texture m_backgroundLeft;
    Texture m_backgroundRight;
};
