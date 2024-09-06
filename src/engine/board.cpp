#include "board.hpp"
#include "pieces/piece.cpp"


Board::Board()
{}


Board::Board(const char* FEN)
{
    int row_i = 0;
    int col_i = 0;

    int i = 1;
    int step = 0;
    for (char Char = *FEN; Char != '\0'; Char = *(FEN + i), ++i) {
        switch (step) {
            case 0: {
                if (std::isdigit(Char)) {
                    row_i += (int)Char - (int)'0';
                }
                else if (Char == '/') {
                    row_i = 0;
                    ++col_i;
                }
                else if (Char == ' ') {
                    ++step;
                }
                else {
                    placePiece(bitboards[Char], col_i * 8 + row_i);
                    ++row_i;
                }
                break;
            }

            case 1: {
                if (Char == 'w')
                    isWhiteTurn = true;
                else if (Char == 'b')
                    isWhiteTurn = false;
                ++step;
                break;
            }
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
