#include "board.hpp"


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
                    bitboards[PieceToInt(Pieces::getPieceTypeFromChar(Char))] |= 1ULL << (col_i * 8 + row_i);
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


const char* Board::generateFEN()
{
    std::string m_FEN = "";

    for (int col_i = 0; col_i < 8; ++col_i) {
        // Number of empty squares counter
        int emptyCount = 0;

        for (int row_i = 0; row_i < 8; ++row_i) {
            int bitLocationMask = 1ULL << (col_i * 8 + row_i);
            bool pieceFound = false;

            // Loop through all bitboards
            for (int i = 0; i < PieceCount; ++i) {
                Pieces::PieceType pieceType = static_cast<Pieces::PieceType>(i);
                Bitboard bitboard = bitboards[i];

                if (bitboard & bitLocationMask) {
                    if (emptyCount)
                        m_FEN += (char)(emptyCount + 48);

                    emptyCount = 0;
                    m_FEN += Pieces::getPieceChar(pieceType);

                    pieceFound = true;
                    break;
                }
            }

            if (!pieceFound) {
                ++emptyCount;
            }
        }

        if (emptyCount)
            m_FEN += (char)(emptyCount + 48);

        if (col_i < 7)
            m_FEN += '/';
    }

    return m_FEN.c_str();
}
