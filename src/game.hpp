#pragma once
#include "engine/engine.hpp"
#include "interface/board_renderer.hpp"

#include "interface/button.cpp"


class Game
{
public:
    Game();

    void update();
    void render();

private:
    // 0: Main menu
    // 1: Play menu
    // 2: Game
    char m_mode = 0;
    Engine m_engine;
    Renderer m_renderer;

    Button m_play;

    Button m_vsBot;
    Button m_vsFriend;
    Button m_back;

    Texture m_backgroundLeft;
    Texture m_backgroundRight;
};
