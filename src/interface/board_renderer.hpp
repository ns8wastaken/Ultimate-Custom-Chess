#pragma once
#include <raylib.h>
#include <cctype>
#include <cstdint>
#include <array>

#include "constants.hpp"
#include "settings.hpp"
#include "pieces.hpp"


typedef uint64_t Bitboard;

#ifndef PIECE_COUNT
#define PIECE_COUNT
constexpr int PieceCount = static_cast<int>(Pieces::PieceType::PieceCount);
#endif


class Renderer
{
public:
    Renderer();

    void render(const char* FEN,
                const Bitboard& engineSelectedPiece,
                const Bitboard& engineSelectedPieceMoves,
                const Bitboard& white,
                const Bitboard& black);

    std::array<Texture, (size_t)PieceCount> m_textures;

private:
    void m_loadPieceTextures();
    void m_generateBoardBackground();

    int m_backgroundColorIndex = 0;

    Texture m_boardTexture;

    void m_renderPieces(const char* FEN);
};
