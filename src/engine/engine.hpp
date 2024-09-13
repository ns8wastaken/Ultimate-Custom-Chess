#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include <limits>

#include "constants.hpp"
#include "board.hpp"


class Engine
{
public:
    Engine(const char* FEN);

    void update(const Vector2& mousePos);


    const char* c_getFEN();
    const Bitboard& c_getSelectedPiece();
    const Bitboard& c_getSelectedPieceMoves();

    const Bitboard& c_getOccupiedSquaresWhite();
    const Bitboard& c_getOccupiedSquaresBlack();

    void setGamemode(bool isVsBot);

private:
    int quiescentSearch(int alpha, int beta);
    int negaMax(int alpha);

    Board m_board;
    bool m_isVsBot = true;

    Bitboard m_selectedPiecePos = 0ULL;
    Pieces::PieceType m_selectedPieceType = Pieces::PieceType::None;
    Bitboard m_selectedPieceMoves = 0ULL;

    Bitboard m_generateMove(
        const Bitboard& position,
        const Pieces::PieceType& pieceType,
        const Bitboard& occupiedSquaresWhite,
        const Bitboard& occupiedSquaresBlack) const;
    std::vector<Pieces::Move> m_generateBotMoves() const;

    std::string m_FEN = "";
    bool m_requiresNewFEN;
    void m_generateFEN();

    int m_evaluateBoard();
};
