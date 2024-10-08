#include "board_renderer.hpp"


Renderer::Renderer()
{
    m_loadPieceTextures();
    m_generateBoardBackground();
}


void Renderer::m_loadPieceTextures()
{
    // Loop through all pieces and cache textures
    for (int i = 0; i < PieceCount; ++i) {
        // Get piece type
        Pieces::PieceType pieceType = static_cast<Pieces::PieceType>(i);

        // Load texture
        Image image = LoadImage(Pieces::getPieceSpritePath(pieceType));

        // Cache texture
        m_textures[i] = LoadTextureFromImage(image);
        UnloadImage(image);
    }
}


void Renderer::m_generateBoardBackground()
{
    Image chessboard = GenImageColor(Constants::ScreenSize, Constants::ScreenSize, WHITE);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if ((x + y) % 2 == 0) {
                ImageDrawRectangle(
                    &chessboard,
                    x * Constants::SquareSize,
                    y * Constants::SquareSize,
                    Constants::SquareSize,
                    Constants::SquareSize,
                    Settings::BackgroundColors[m_backgroundColorIndex].dark
                );
            }
            else {
                ImageDrawRectangle(
                    &chessboard,
                    x * Constants::SquareSize,
                    y * Constants::SquareSize,
                    Constants::SquareSize,
                    Constants::SquareSize,
                    Settings::BackgroundColors[m_backgroundColorIndex].light
                );
            }
        }
    }

    m_boardTexture = LoadTextureFromImage(chessboard);

    UnloadImage(chessboard);
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
            Texture texture = m_textures[PieceToInt(PieceFromChar(Char))];

            float sizeMult = Pieces::getPieceSizeMult(Char);

            float centeringFactor = (float)Constants::SquareSize / 2.0f - (float)Constants::SquareSize * sizeMult / 2.0f;

            Vector2 pos = {
                (float)(row_i * Constants::SquareSize) + centeringFactor,
                (float)(col_i * Constants::SquareSize) + centeringFactor
            };

            DrawTextureEx(texture, pos, 0, (float)Constants::SquareSize / texture.width * sizeMult, WHITE);
            ++row_i;
        }
    }
}


void Renderer::render(
    const char* FEN,
    const Bitboard& engineSelectedPiece,
    const Bitboard& engineSelectedPieceMoves,
    const Bitboard& white,
    const Bitboard& black
)
{
    DrawTextureEx(m_boardTexture, Vector2{0, 0}, 0, 1, Color{255, 255, 255, 255});



    // TO BE REMOVED ##########################################################################################################
    // White occupied squares
    Bitboard whites = white;
    int zeroswhite = __builtin_ctzll(whites);
    while (zeroswhite < 64) {
        Vector2 pos = {(float)((zeroswhite % 8) * Constants::SquareSize), (float)((zeroswhite / 8) * Constants::SquareSize)};
        DrawRectangleV(pos, Vector2{Constants::SquareSize, Constants::SquareSize}, Color{255, 0, 255, 120});

        whites ^= (1ULL << zeroswhite);
        zeroswhite = __builtin_ctzll(whites);
    }

    // Black occupied squares
    Bitboard blacks = black;
    int zerosblack = __builtin_ctzll(blacks);
    while (zerosblack < 64) {
        Vector2 pos = {(float)((zerosblack % 8) * Constants::SquareSize), (float)((zerosblack / 8) * Constants::SquareSize)};
        DrawRectangleV(pos, Vector2{Constants::SquareSize, Constants::SquareSize}, Color{0, 255, 255, 120});

        blacks ^= (1ULL << zerosblack);
        zerosblack = __builtin_ctzll(blacks);
    }
    // ########################################################################################################################




    // Highlight selected piece
    int zeros = __builtin_ctzll(engineSelectedPiece);
    Vector2 pos = {(float)((zeros % 8) * Constants::SquareSize), (float)((zeros / 8) * Constants::SquareSize)};
    DrawRectangleV(pos, Vector2{Constants::SquareSize, Constants::SquareSize}, Color{255, 255, 0, 160});


    m_renderPieces(FEN);


    // Highlight selected piece's moves
    Bitboard moves = engineSelectedPieceMoves;
    zeros = __builtin_ctzll(moves);
    while (zeros < 64) {
        Vector2 pos = {(float)((zeros % 8) * Constants::SquareSize), (float)((zeros / 8) * Constants::SquareSize)};
        DrawRectangleV(pos, Vector2{Constants::SquareSize, Constants::SquareSize}, Color{255, 0, 0, 120});

        moves ^= (1ULL << zeros);
        zeros = __builtin_ctzll(moves);
    }
}
