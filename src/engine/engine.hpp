#pragma once
#include <raylib.h>
#include <vector>
#include <string>

#include "constants.hpp"
#include "board.hpp"


class Engine
{
public:
    Engine(const char* FEN);

    void update(Vector2 mousePos);

    // std::vector<Bitboard> generateMoves() const;
    Bitboard generateMove(
        Bitboard position,
        Pieces::PieceType pieceType,
        Bitboard& occupiedSquaresWhite,
        Bitboard& occupiedSquaresBlack) const;

    const char* c_getFEN();
    const Bitboard& c_getSelectedPiece();
    const Bitboard& c_getSelectedPieceMoves();

private:
    Board m_board;

    Bitboard m_selectedPiecePos = 0ULL;
    Pieces::PieceType m_selectedPieceType = Pieces::PieceType::None;
    Bitboard m_selectedPieceMoves = 0ULL;

    std::string m_FEN = "";
    bool m_requiresNewFEN;
    void m_generateFEN();

    int evaluateBoard();
};
