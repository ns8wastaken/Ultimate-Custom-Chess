#include "renderer.hpp"


Renderer::Renderer() : m_boardShader(LoadShader(0, "src/interface/shaders/board.fs"))
{
    m_loadPieceTextures();

    // Generate blank texture for shader
    Image imBlank = GenImageColor(Constants::ScreenSize, Constants::ScreenSize, BLANK);
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
            Vector2 pos = { (float)(row_i * Constants::SquareSize), (float)(col_i * Constants::SquareSize) };
            DrawTextureEx(m_textures[PieceToInt(PieceFromChar(Char))], pos, 0, (float)Constants::SquareSize / 32.0f, WHITE);
            ++row_i;
        }
    }
}


void Renderer::render(const char* FEN, const Bitboard& engineSelectedPiece, const Bitboard& engineSelectedPieceMoves, const Bitboard& white, const Bitboard& black)
{
    m_renderBoardBackground();


    Bitboard whites = white;
    int zeroswhite = __builtin_ctzll(whites);
    while (zeroswhite < 64) {
        Vector2 pos = { (float)((zeroswhite % 8) * Constants::SquareSize), (float)((zeroswhite / 8) * Constants::SquareSize) };
        DrawRectangleV(pos, Vector2{ Constants::SquareSize, Constants::SquareSize }, Color{ 255, 0, 255, 120 });

        whites ^= (1ULL << zeroswhite);
        zeroswhite = __builtin_ctzll(whites);
    }


    Bitboard blacks = black;
    int zerosblack = __builtin_ctzll(blacks);
    while (zerosblack < 64) {
        Vector2 pos = { (float)((zerosblack % 8) * Constants::SquareSize), (float)((zerosblack / 8) * Constants::SquareSize) };
        DrawRectangleV(pos, Vector2{ Constants::SquareSize, Constants::SquareSize }, Color{ 0, 255, 255, 120 });

        blacks ^= (1ULL << zerosblack);
        zerosblack = __builtin_ctzll(blacks);
    }




    // Highlight selected piece
    int zeros = __builtin_ctzll(engineSelectedPiece);
    Vector2 pos = { (float)((zeros % 8) * Constants::SquareSize), (float)((zeros / 8) * Constants::SquareSize) };
    DrawRectangleV(pos, Vector2{ Constants::SquareSize, Constants::SquareSize }, Color{ 255, 255, 0, 160 });

    m_renderPieces(FEN);

    // Highlight selected piece's moves
    Bitboard moves = engineSelectedPieceMoves;
    zeros = __builtin_ctzll(moves);
    while (zeros < 64) {
        Vector2 pos = { (float)((zeros % 8) * Constants::SquareSize), (float)((zeros / 8) * Constants::SquareSize) };
        DrawRectangleV(pos, Vector2{ Constants::SquareSize, Constants::SquareSize }, Color{ 255, 0, 0, 120 });

        moves ^= (1ULL << zeros);
        zeros = __builtin_ctzll(moves);
    }
}
