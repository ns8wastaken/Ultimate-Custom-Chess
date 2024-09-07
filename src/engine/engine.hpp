#pragma once
#include <raylib.h>

#include "constants.hpp"
#include "board.hpp"


class Engine
{
public:
    Engine(const char* FEN);

    void update(Vector2 mousePos);

    std::vector<Bitboard> generateMoves() const;

    const char* c_getFEN();
    const Pieces::Move* c_getCurrentMove();

private:
    Board m_board;
    Pieces::Move m_currentMove;

    std::string m_FEN = "";
    bool m_requiresNewFEN;
    void m_generateFEN();

    int evaluateBoard();
};
