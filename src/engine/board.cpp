#include "board.hpp"
#include <bitset>


Board::Board(const char* FEN)
{
    bitboards.fill(0ULL);

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
                    bitboards[PieceToInt(PieceFromChar(Char))] |= 1ULL << (col_i * 8 + row_i);
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


std::vector<Bitboard> Board::generateMoves(Bitboard& bitboard, int index) const
{
    Pieces::PieceType pieceType = static_cast<Pieces::PieceType>(index);
}
