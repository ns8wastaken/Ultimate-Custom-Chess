#include "board.hpp"
#include <bitset>


Board::Board(const char* FEN)
{
    m_precomputeMoves();
    m_loadFEN(FEN);
}


void Board::makeMove(Pieces::PieceType pieceType, Bitboard from, Bitboard to)
{
    bitboards[PieceToInt(pieceType)] &= ~from;
    bitboards[PieceToInt(pieceType)] |= to;
}


void Board::m_precomputeMoves()
{
    precomputedMoves.knightMoves.fill(0ULL);
    precomputedMoves.kingMoves.fill(0ULL);

    precomputedMoves.cubistMoves.fill(0ULL);

    /*
        Bit bitmasks

            BitMaskA           BitMaskB
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B

            BitMaskA2          BitMaskB2
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
    */

    uint64_t BitMaskA = ~0x8080808080808080ULL;
    uint64_t BitMaskA2 = ~0xc0c0c0c0c0c0c0c0ULL;

    uint64_t BitMaskB = ~0x101010101010101ULL;
    uint64_t BitMaskB2 = ~0x303030303030303ULL;

    for (uint64_t i = 0; i < 63; ++i) {
        // Knight
        precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, 17) & BitMaskB;
        precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, 15) & BitMaskA;
        precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, 10) & BitMaskB2;
        precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, 6) & BitMaskA2;
        precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, -6) & BitMaskB2;
        precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, -10) & BitMaskA2;
        precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, -15) & BitMaskB;
        precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, -17) & BitMaskA;


        // King
        precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, 9) & BitMaskB;
        precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, 8);
        precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, 7) & BitMaskA;
        precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, 1) & BitMaskB;
        precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, -1) & BitMaskA;
        precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, -7) & BitMaskB;
        precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, -8);
        precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, -9) & BitMaskA;


        // Cubist
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 18) & BitMaskB2;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 10) & BitMaskB2;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 2) & BitMaskB2;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -6) & BitMaskB2;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -14) & BitMaskB2;

        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 14) & BitMaskA2;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 6) & BitMaskA2;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -2) & BitMaskA2;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -10) & BitMaskA2;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -18) & BitMaskA2;

        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 17) & BitMaskB;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 15) & BitMaskA;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -15) & BitMaskB;
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -17) & BitMaskA;

        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 16);
        precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -16);
    }
}


void Board::m_loadFEN(const char* FEN)
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
