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

    void update(const Vector2& mousePos);

    // std::vector<Bitboard> generateMoves() const;
    Bitboard generateMove(
        const Bitboard& position,
        const Pieces::PieceType& pieceType,
        const Bitboard& occupiedSquaresWhite,
        const Bitboard& occupiedSquaresBlack) const;

    const char* c_getFEN();
    const Bitboard& c_getSelectedPiece();
    const Bitboard& c_getSelectedPieceMoves();

    const Bitboard& c_getOccupiedSquaresWhite();
    const Bitboard& c_getOccupiedSquaresBlack();

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
