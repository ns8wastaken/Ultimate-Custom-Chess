#pragma once
#include <cstdint>
#include <cctype>
#include <ctype.h>
#include <array>
#include <vector>
#include <string>

#include "pieces.hpp"

typedef uint64_t Bitboard;

#ifndef PIECE_COUNT
  #define PIECE_COUNT
constexpr int PieceCount = static_cast<int>(Pieces::PieceType::PieceCount);
#endif

typedef std::array<Bitboard, static_cast<size_t>(PieceCount)> BitboardArray;


class Board
{
public:
    Board(const char* FEN);

    BitboardArray bitboards;

    // Move generator
    std::vector<Bitboard> generateMoves(Bitboard& bitboard, int index) const;

private:
    std::string m_FEN;

    bool isWhiteTurn;
};
