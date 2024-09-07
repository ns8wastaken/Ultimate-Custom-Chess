#pragma once
#include <raylib.h>
#include <unordered_map>
#include <cctype>
#include <cstdint>

#include "pieces.hpp"


typedef uint64_t Bitboard;

#ifndef PIECE_COUNT
  #define PIECE_COUNT
constexpr int PieceCount = static_cast<int>(Pieces::PieceType::PieceCount);
#endif

typedef std::array<Bitboard, static_cast<size_t>(PieceCount)> BitboardArray;


class Renderer
{
public:
    Renderer(int screenWidth, int screenHeight);

    void render(float deltaTime, const char* FEN);

    std::array<Texture, static_cast<size_t>(PieceCount)> m_textures;

private:
    void m_loadPieceTextures();

    Shader m_boardShader;
    Texture m_textureBlank;

    void m_renderBoardBackground();
    void m_renderPieces(const char* FEN);
};
