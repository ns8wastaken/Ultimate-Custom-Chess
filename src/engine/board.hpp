#pragma once
#include <cstdint>
#include <cctype>
#include <array>
#include <stack>

#include "utils.hpp"
#include "pieces.hpp"


typedef uint64_t Bitboard;


#ifndef PIECE_COUNT
#define PIECE_COUNT
constexpr int PieceCount = static_cast<int>(Pieces::PieceType::PieceCount);
#endif

typedef std::array<Bitboard, static_cast<size_t>(PieceCount)> BitboardArray;


class Board
{
private:
    struct PrecomputedMoves
    {
        std::array<Bitboard, 64> knightMoves;
        std::array<Bitboard, 64> kingMoves;
        std::array<Bitboard, 64> cubistMoves;
        std::array<Bitboard, 64> foolMoves;
        std::array<Bitboard, 64> godMoves;
    };

public:
    Board(const char* FEN);

    void makeMove(const Pieces::PieceType pieceType, const Bitboard& from, const Bitboard& to, bool addToHistory);

    BitboardArray bitboards;
    PrecomputedMoves precomputedMoves;

    // Number of plies (half moves) played
    int depthPly = 0;
    // Number of moves played
    int depthMove = 0;

    std::array<Pieces::PieceType, 64> pieceLookup;

    bool isWhiteTurn = true;
    Bitboard occupiedSquaresWhite = 0ULL;
    Bitboard occupiedSquaresBlack = 0ULL;

    Bitboard initialPawnsWhite = 0ULL;
    Bitboard initialPawnsBlack = 0ULL;

    void undoMove();

private:
    // From: en passant hitbox
    // To: current position
    Pieces::Move enPassant;

    std::stack<Pieces::HistoryMove> m_moveHistory{};

    void m_precomputeMoves();
    void m_loadFEN(const char* FEN);
};
