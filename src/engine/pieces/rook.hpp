#pragma once
#include "piece.hpp"


class Rook : public Piece
{
public:
    Rook(PieceColor c, bool* isWhiteTurn_) : Piece(c, isWhiteTurn_) {}

    Bitboard generateMoves(Bitboard occupiedSquaresWhite, Bitboard occupiedSquaresBlack, Bitboard position) const override
    {
        Bitboard moves = 0ULL;

        int offsets[] = { 1, 7, 8, 9 };

        int zeroCount = 0;
        while (position >> zeroCount) {
            ++zeroCount;
        }
        --zeroCount;

        // Left
        for (int i = 1; i < 8 - (zeroCount % 8); ++i) {
            moves |= position << i;
        }
        // Right
        for (int i = 1; i < (zeroCount % 8) + 1; ++i) {
            moves |= position >> i;
        }
        // Up
        for (int i = 1; i < 8 - (zeroCount / 8); ++i) {
            moves |= position << 8 * i;
        }
        // Down
        for (int i = 1; i < (zeroCount / 8) + 1; ++i) {
            moves |= position >> 8 * i;
        }

        return moves & ((color == PieceColor::White) ? ~occupiedSquaresWhite : ~occupiedSquaresBlack);
    }

    char getChar() const override
    {
        return color == PieceColor::White ? 'R' : 'r';
    }
};
