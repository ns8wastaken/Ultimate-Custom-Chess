#include "renderer.hpp"


Renderer::Renderer(int screenWidth, int screenHeight, const Board& board)
    : m_board(board), m_boardShader(LoadShader(0, "src/interface/shaders/board.fs"))
{
    m_loadPieceTextures();

    // Generate blank texture for shader
    Image imBlank = GenImageColor(screenWidth, screenHeight, BLANK);
    m_textureBlank = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);
}


void Renderer::m_loadPieceTextures()
{
    for (auto& [Char, bitboard] : m_board.bitboards) {
        if (Char == '\0') continue;

        Image image = LoadImage(Piece::getImagePath(Char));
        Texture texture = LoadTextureFromImage(image);
        UnloadImage(image);

        m_textures[Char] = texture;
    }
}


void Renderer::m_renderBoardBackground()
{
    BeginShaderMode(m_boardShader);
    DrawTextureV(m_textureBlank, Vector2{ 0, 0 }, WHITE);
    EndShaderMode();
}


void Renderer::m_renderPieces()
{
    for (auto& [Char, bitboard] : m_board.bitboards) {
        if (Char == '\0') continue;

        for (uint64_t i = 0; i < 64; ++i) {
            if (bitboard & (1ULL << i)) {
                Vector2 pos = { (i % 8) * 100.0f, (float)(i / 8) * 100.0f };
                DrawTextureEx(m_textures[Char], pos, 0, 100 / 32, WHITE);
            }
        }
    }
}


void Renderer::render()
{
    m_renderBoardBackground();
    m_renderPieces();
}
