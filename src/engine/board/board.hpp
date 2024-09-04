#pragma once
#include <array>

#include "pieces.hpp"


typedef Pieces::Piece Piece;


class Board
{
public:
    Board();
    Board(const char* FEN);

private:
    std::array<Piece, static_cast<unsigned char>(Piece::PieceCount)> pieces;

    void assertPieceChars();
};
