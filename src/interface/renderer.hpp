#pragma once
#include <raylib.h>
#include <unordered_map>
#include <cstdint>


typedef uint64_t Bitboard;


class Renderer
{
public:
    Renderer(int screenWidth, int screenHeight, const std::unordered_map<char, Bitboard>& bitboards);

    void render(float deltaTime);

    std::unordered_map<char, Texture> m_textures;

private:
    const std::unordered_map<char, Bitboard>& m_bitboards;

    void m_loadPieceTextures();

    Shader m_boardShader;
    Texture m_textureBlank;

    void m_renderBoardBackground();
    void m_renderPieces();
};
