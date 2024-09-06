#pragma once
#include "piece.hpp"


class King : public Piece
{
public:
    King(PieceColor c, bool *isWhiteTurn_) : Piece(c, isWhiteTurn_) {}

    Bitboard generateMoves(Bitboard occupiedSquaresWhite, Bitboard occupiedSquaresBlack, int position) const override
    {
        Bitboard moves = 0ULL;

        int offsets[] = { 1, 7, 8, 9 };

        for (int i = 0; i < 8; ++i) {
            for (int j = 0; i < 7; ++j) {
                moves |= position << (offsets[i] * j);
                moves |= position > (offsets[i] * j);
            }
        }

        return moves & ((color == PieceColor::White) ? ~occupiedSquaresWhite : ~occupiedSquaresBlack);
    }

    char getChar() const override
    {
        return color == PieceColor::White ? 'K' : 'k';
    }
};
