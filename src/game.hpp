#pragma once
#include "engine/engine.hpp"
#include "interface/board_renderer.hpp"


class Game
{
public:
    Game();

    void update();
    void render();

private:
    char m_mode;
    Engine m_engine;
    Renderer m_renderer;
};
