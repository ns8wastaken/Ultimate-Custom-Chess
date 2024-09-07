#pragma once
#include <raylib.h>

#include "board.hpp"


class Engine
{
public:
    Engine(const char* FEN);

    void update(Vector2 mousePos);

    const BitboardArray& c_getBitboards();
    const char* c_getFEN();

private:
    Board m_board;

    int evaluateBoard();
};
