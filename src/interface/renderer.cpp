#include "renderer.hpp"


Renderer::Renderer(int screenWidth, int screenHeight, const Pieces::Move* engineCurrentMove)
    : m_boardShader(LoadShader(0, "src/interface/shaders/board.fs")), m_engineCurrentMove(engineCurrentMove)
{
    m_loadPieceTextures();

    // Generate blank texture for shader
    Image imBlank = GenImageColor(screenWidth, screenHeight, BLANK);
    m_textureBlank = LoadTextureFromImage(imBlank);
    UnloadImage(imBlank);
}


void Renderer::m_loadPieceTextures()
{
    // Loop through all pieces and cache textures
    for (int i = 0; i < PieceCount; ++i) {
        // Get piece type
        Pieces::PieceType pieceType = static_cast<Pieces::PieceType>(i);

        // Load texture
        Image image = LoadImage(Pieces::getPieceSpritePath(pieceType));
        Texture texture = LoadTextureFromImage(image);
        UnloadImage(image);

        // Cache texture
        m_textures[i] = texture;
    }
}


void Renderer::m_renderBoardBackground()
{
    BeginShaderMode(m_boardShader);
    DrawTextureV(m_textureBlank, Vector2{ 0, 0 }, WHITE);
    EndShaderMode();
}


void Renderer::m_renderPieces(const char* FEN)
{
    int row_i = 0;
    int col_i = 0;

    int i = 1;
    for (char Char = *FEN; Char != '\0'; Char = *(FEN + i), ++i) {
        if (std::isdigit(Char)) {
            row_i += (int)Char - (int)'0';
        }
        else if (Char == '/') {
            row_i = 0;
            ++col_i;
        }
        else {
            Vector2 pos = { row_i * Constants::SquareSize, col_i * Constants::SquareSize };
            DrawTextureEx(m_textures[PieceToInt(PieceFromChar(Char))], pos, 0, (float)Constants::SquareSize / 32.0f, WHITE);
            ++row_i;
        }
    }
}


void Renderer::render(float deltaTime, const char* FEN)
{
    m_renderBoardBackground();

    int zeros = 0;
    while (m_engineCurrentMove->from >> zeros) {
        ++zeros;
    }
    --zeros;

    Vector2 pos = { (zeros % 8) * Constants::SquareSize, (zeros / 8) * Constants::SquareSize };
    DrawRectangleV(pos, Vector2{ Constants::SquareSize, Constants::SquareSize }, Color{ 255, 255, 0, 160 });

    m_renderPieces(FEN);
}
