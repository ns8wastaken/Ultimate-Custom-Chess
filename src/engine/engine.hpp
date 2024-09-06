#pragma once
#include "board.hpp"


class Engine
{
public:
    Engine();

    const std::unordered_map<char, Bitboard>& getBitboard();

private:
    Board m_board;

    int evaluateBoard();
};
