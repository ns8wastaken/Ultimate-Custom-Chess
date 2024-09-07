#pragma once
#include <raylib.h>
#include <cctype>
#include <cstdint>
#include <array>

#include "constants.hpp"
#include "pieces.hpp"


typedef uint64_t Bitboard;

#ifndef PIECE_COUNT
  #define PIECE_COUNT
constexpr int PieceCount = static_cast<int>(Pieces::PieceType::PieceCount);
#endif


class Renderer
{
public:
    Renderer(int screenWidth, int screenHeight, const Pieces::Move* engineCurrentMove);

    void render(float deltaTime, const char* FEN);

    std::array<Texture, static_cast<size_t>(PieceCount)> m_textures;

private:
    void m_loadPieceTextures();

    Shader m_boardShader;
    Texture m_textureBlank;

    void m_renderBoardBackground();
    void m_renderPieces(const char* FEN);

    const Pieces::Move* m_engineCurrentMove;
};
