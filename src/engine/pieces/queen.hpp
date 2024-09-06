#pragma once
#include "piece.hpp"


class King : public Piece
{
public:
    King(PieceColor c, bool *isWhiteTurn_) : Piece(c, isWhiteTurn_) {}

    Bitboard generateMoves(Bitboard occupiedSquaresWhite, Bitboard occupiedSquaresBlack, Bitboard position) const override
    {
        Bitboard moves = 0ULL;

        int offsets[] = { 1, 7, 8, 9 };

        int zeros = 0;
        {
            Bitboard temp = position;
            while (temp) {
                temp >>= 1;
                ++zeros;
            }
        }

        for (int i = 0; i < 7 - (position % 8); ++i) {
            moves |= position << i;
        }
        for (int i = 0; i < (position % 8); ++i) {
            moves |= position >> i;
        }
        for (int i = 0; i < 7 - (position / 8); ++i) {
            moves |= position << 8 * i;
        }
        for (int i = 0; i < (position / 8); ++i) {
            moves |= position >> 8 * i;
        }


        return moves & ((color == PieceColor::White) ? ~occupiedSquaresWhite : ~occupiedSquaresBlack);
    }

    char getChar() const override
    {
        return color == PieceColor::White ? 'K' : 'k';
    }
};
