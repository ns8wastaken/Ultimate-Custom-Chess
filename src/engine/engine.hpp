#pragma once
#include "board.hpp"


class Engine
{
public:
    Engine();

    const Board& getBoard();

private:
    Board m_board;

    int evaluateBoard();
};
