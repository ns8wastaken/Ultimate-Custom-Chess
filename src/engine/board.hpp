#pragma once
#include <unordered_map>
#include <cstdint>
#include <ctype.h>

#include "pieces/piece.hpp"
#include "pieces/king.hpp"


class Board
{
public:
    Board();
    Board(const char *FEN);

    std::unordered_map<char, Bitboard> bitboards = {
        { 'K', 0ULL },
        { 'k', 0ULL },
        { 'Q', 0ULL },
        { 'q', 0ULL }
    };

    Bitboard generateMoves(int position, PieceColor color) const;

    void placePiece(Bitboard& board, int position);
    void removePiece(Bitboard& board, int position);

private:
    bool isWhiteTurn;

    struct Move
    {
        Bitboard bitboard;
    };
};
