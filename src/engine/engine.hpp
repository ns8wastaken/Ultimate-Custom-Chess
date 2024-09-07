#pragma once
#include <raylib.h>

#include "constants.hpp"
#include "board.hpp"


class Engine
{
public:
    Engine(const char* FEN);

    void update(Vector2 mousePos);

    const char* c_getFEN();

private:
    Board m_board;

    std::string m_FEN = "";
    bool m_requiresNewFEN;
    void m_generateFEN();

    int evaluateBoard();
};
