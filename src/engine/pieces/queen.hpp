#pragma once
#include "piece.hpp"


class King : public Piece
{
public:
    King(PieceColor c, bool* isWhiteTurn_) : Piece(c, isWhiteTurn_) {}

    Bitboard generateMoves(Bitboard occupiedSquaresWhite, Bitboard occupiedSquaresBlack, Bitboard position) const override
    {
        Bitboard moves = 0ULL;

        int offsets[] = { 1, 7, 8, 9 };

        int zeros = 0;
        while (position >> zeros) {
            ++zeros;
        }
        --zeros;

        int distLeft = 8 - (zeros % 8);
        int distRight = (zeros % 8) + 1;
        int distUp = 8 - (zeros / 8);
        int distDown = (zeros / 8) + 1;

        // Left
        for (int i = 1; i < distLeft; ++i) {
            moves |= position << i;
        }
        // Right
        for (int i = 1; i < distRight; ++i) {
            moves |= position >> i;
        }
        // Up
        for (int i = 1; i < distUp; ++i) {
            moves |= position << 8 * i;
        }
        // Down
        for (int i = 1; i < distDown; ++i) {
            moves |= position >> 8 * i;
        }
        // Top left
        for (int i = 1; i < ((distLeft < distUp) ? distLeft : distUp); ++i) {
            moves |= position << 9 * i;
        }
        // Top right
        for (int i = 1; i < ((distUp < distRight) ? distUp : distRight); ++i) {
            moves |= position << 7 * i;
        }
        // Bottom right
        for (int i = 1; i < ((distRight < distDown) ? distRight : distDown); ++i) {
            moves |= position >> 9 * i;
        }
        // Bottom left
        for (int i = 1; i < ((distDown < distLeft) ? distDown : distLeft); ++i) {
            moves |= position >> 7 * i;
        }

        return moves & ((color == PieceColor::White) ? ~occupiedSquaresWhite : ~occupiedSquaresBlack);
    }

    char getChar() const override
    {
        return color == PieceColor::White ? 'Q' : 'q';
    }
};
