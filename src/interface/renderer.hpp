#pragma once
#include <raylib.h>


class Board;

class Renderer
{
public:
    Renderer(int screenWidth, int screenHeight, const Board& board);

    void render();

    std::unordered_map<char, Texture> m_textures;
private:
    const Board& m_board;

    void m_loadPieceTextures();

    Shader m_boardShader;
    Texture m_textureBlank;

    void m_renderBoardBackground();
    void m_renderPieces();
};
