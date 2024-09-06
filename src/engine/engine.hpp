#pragma once
#include "board.hpp"


class Engine
{
public:
    Engine();

    void update(Vector2 mousePos);

    const std::unordered_map<char, Bitboard>& c_getBitboard();

private:
    Board m_board;

    int evaluateBoard();
};
