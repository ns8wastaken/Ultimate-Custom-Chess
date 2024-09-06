#include "board.hpp"
#include "pieces/piece.cpp"
#include <iostream>


Board::Board()
{}


Board::Board(const char* FEN)
{
    int row_i = 0;
    int col_i = 0;

    int i = 1;
    for (char Char = *FEN; Char != '\0'; Char = *(FEN + i), ++i) {
        if (std::isdigit(Char)) {
            row_i += (int)Char - (int)'0';
        }
        else if (Char == '/') {
            row_i = 0;
            ++col_i;
        }
        else {
            placePiece(bitboards[Char], col_i * 8 + row_i);

            ++row_i;
        }
    }
}


Bitboard Board::generateMoves(int position, PieceColor color) const
{
    return 0;
}


void Board::placePiece(Bitboard& board, int position)
{
    board |= 1ULL << position;
}


void Board::removePiece(Bitboard& board, int position)
{
    board &= ~(1ULL << position);
}
