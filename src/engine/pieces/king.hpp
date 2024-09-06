#pragma once
#include "piece.hpp"


class King : public Piece
{
public:
    King(PieceColor c, bool *isWhiteTurn_) : Piece(c, isWhiteTurn_) {}

    Bitboard generateMoves(Bitboard occupiedSquaresWhite, Bitboard occupiedSquaresBlack, Bitboard position) const override
    {
        Bitboard moves = 0ULL;

        moves |= position << 9;
        moves |= position << 8;
        moves |= position << 7;
        moves |= position << 1;
        moves |= position >> 1;
        moves |= position >> 7;
        moves |= position >> 8;
        moves |= position >> 9;

        return moves & ((color == PieceColor::White) ? ~occupiedSquaresWhite : ~occupiedSquaresBlack);
    }

    char getChar() const override
    {
        return color == PieceColor::White ? 'K' : 'k';
    }
};
