#include "engine.hpp"
#include "board.cpp"
#include <iostream>


Engine::Engine(const char* FEN)
    : m_board(FEN), m_requiresNewFEN(true)
{}


void Engine::update(Vector2 mousePos)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int clickRow_i = (int)(mousePos.x / Constants::SquareSize);
        int clickCol_i = (int)(mousePos.y / Constants::SquareSize);

        // Out of bounds
        if ((unsigned)clickRow_i >= 8 || (unsigned)clickCol_i >= 8) {
            return;
        }

        uint64_t bitLocationMask = 1ULL << (clickCol_i * 8 + clickRow_i);

        // Checks to see if piece was clicked
        for (int i = 0; i < PieceCount; ++i) {
            Pieces::PieceType pieceType = static_cast<Pieces::PieceType>(i);
            Bitboard bitboard = m_board.bitboards[i];

            // If piece was clicked
            if (bitboard & bitLocationMask) {
                if (m_currentMove.from == 0ULL) {
                    m_currentMove.from = bitLocationMask;
                }
                else {
                    // TODO: Send move to board to make the move
                    m_currentMove.to = bitLocationMask;
                    m_currentMove.from = 0ULL;
                    m_currentMove.to = 0ULL;
                }

                return;
            }
        }

        // This happens if no piece was clicked (empty square)
        m_currentMove.from = 0ULL;
    }
}


int Engine::evaluateBoard()
{
    return 0.0;
}


std::vector<Bitboard> Engine::generateMoves() const
{
    std::vector<Bitboard> moves{};
}


const Pieces::Move* Engine::c_getCurrentMove()
{
    return &m_currentMove;
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
