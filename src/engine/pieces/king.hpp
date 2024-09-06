#pragma once
#include "piece.hpp"


class King : public Piece
{
public:
    King(PieceColor c) : Piece(c) {}

    Bitboard generateMoves(Bitboard occupiedSquaresWhite, Bitboard occupiedSquaresBlack, int position) const override
    {
        Bitboard moves = position;

        moves |= moves - 9;
        moves |= moves - 8;
        moves |= moves - 7;
        moves |= moves - 1;
        moves |= moves + 1;
        moves |= moves + 7;
        moves |= moves + 8;
        moves |= moves + 9;

        return moves & ((color == PieceColor::White) ? ~occupiedSquaresWhite : ~occupiedSquaresBlack);
    }

    char getChar() const override
    {
        return color == PieceColor::White ? 'K' : 'k';
    }
};
