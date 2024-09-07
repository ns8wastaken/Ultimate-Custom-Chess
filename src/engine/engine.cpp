#include "engine.hpp"
#include "board.cpp"


Engine::Engine(const char* FEN)
    : m_board(FEN), m_requiresNewFEN(true)
{}


void Engine::update(Vector2 mousePos)
{
    int clickRow_i = (int)(mousePos.x / Constants::SquareWidth);
    int clickCol_i = (int)(mousePos.y / Constants::SquareHeight);
}


int Engine::evaluateBoard()
{
    return 0.0;
}


void Engine::m_generateFEN()
{
    m_FEN.clear();

    for (int col_i = 0; col_i < 8; ++col_i) {
        // Number of empty squares counter
        int emptyCount = 0;

        for (int row_i = 0; row_i < 8; ++row_i) {
            uint64_t bitLocationMask = 1ULL << (col_i * 8 + row_i);
            bool pieceFound = false;

            // Loop through all bitboards
            for (int i = 0; i < PieceCount; ++i) {
                Pieces::PieceType pieceType = static_cast<Pieces::PieceType>(i);
                Bitboard bitboard = m_board.bitboards[i];

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
}


const char* Engine::c_getFEN()
{
    if (m_requiresNewFEN) {
        m_generateFEN();
        m_requiresNewFEN = false;
    }

    return m_FEN.c_str();
}
