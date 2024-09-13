#include "game.hpp"
#include "engine/engine.cpp"
#include "interface/board_renderer.cpp"


Game::Game()
    : m_engine("rnbqkbnr/ppggpppp/8/8/8/8/PPPPGGPP/RNBQKBNR w"),
      m_play(Vector2{ Constants::ScreenSize / 2, Constants::ScreenSize / 2 }, "assets/ui/buttons/play.png"),
      m_vsBot(Vector2{ Constants::ScreenSize / 2, Constants::ScreenSize / 2 - Constants::ButtonSize * 11 }, "assets/ui/buttons/vs_bot.png"),
      m_vsFriend(Vector2{ Constants::ScreenSize / 2, Constants::ScreenSize / 2 + Constants::ButtonSize * 11 }, "assets/ui/buttons/vs_friend.png"),
      m_back(Vector2{ Constants::ScreenSize / 2, Constants::ScreenSize / 2 + Constants::ButtonSize * 33 }, "assets/ui/buttons/back.png")
{
    Image image = LoadImage("assets/ui/background_left.png");
    m_backgroundLeft = LoadTextureFromImage(image);
    UnloadImage(image);

    image = LoadImage("assets/ui/background_right.png");
    m_backgroundRight = LoadTextureFromImage(image);
    UnloadImage(image);
}


void Game::update()
{
    Vector2 mousePos = GetMousePosition();
    switch (m_mode) {
        case 0: {
            if (m_play.update(mousePos)) {
                m_mode = 1;
            }
        } break;

        case 1: {
            if (m_vsBot.update(mousePos)) {
                m_engine.setGamemode(true);
                m_mode = 2;
            }

            if (m_vsFriend.update(mousePos)) {
                m_engine.setGamemode(false);
                m_mode = 2;
            }

            if (m_back.update(mousePos)) {
                m_mode = 0;
            }
        } break;

        case 2: m_engine.update(mousePos); break;
    }
}


void Game::render()
{
    BeginDrawing();

    switch (m_mode) {
        case 0: {
            ClearBackground(Color{ 0, 0, 0, 255 });

            float pos = (float)((int)(GetTime() * 50.0f) % Constants::ScreenSize);

            DrawTextureEx(m_backgroundLeft,
                          Vector2{ 0, pos },
                          0,
                          Constants::BackgroundSize,
                          Color{ 255, 255, 255, 255 });
            DrawTextureEx(m_backgroundLeft,
                          Vector2{ 0, pos - Constants::ScreenSize },
                          0,
                          Constants::BackgroundSize,
                          Color{ 255, 255, 255, 255 });

            DrawTextureEx(m_backgroundRight,
                          Vector2{ Constants::ScreenSize / 2, -pos },
                          0,
                          Constants::BackgroundSize,
                          Color{ 255, 255, 255, 255 });
            DrawTextureEx(m_backgroundRight,
                          Vector2{ Constants::ScreenSize / 2, Constants::ScreenSize - pos },
                          0,
                          Constants::BackgroundSize,
                          Color{ 255, 255, 255, 255 });

            m_play.render();

        } break;

        case 1: {
            ClearBackground(Color{ 0, 0, 0, 255 });

            float pos = (float)((int)(GetTime() * 50.0f) % Constants::ScreenSize);

            DrawTextureEx(m_backgroundLeft,
                          Vector2{ 0, pos },
                          0,
                          Constants::BackgroundSize,
                          Color{ 255, 255, 255, 255 });
            DrawTextureEx(m_backgroundLeft,
                          Vector2{ 0, pos - Constants::ScreenSize },
                          0,
                          Constants::BackgroundSize,
                          Color{ 255, 255, 255, 255 });

            DrawTextureEx(m_backgroundRight,
                          Vector2{ Constants::ScreenSize / 2, -pos },
                          0,
                          Constants::BackgroundSize,
                          Color{ 255, 255, 255, 255 });
            DrawTextureEx(m_backgroundRight,
                          Vector2{ Constants::ScreenSize / 2, Constants::ScreenSize - pos },
                          0,
                          Constants::BackgroundSize,
                          Color{ 255, 255, 255, 255 });


            m_vsBot.render();
            m_vsFriend.render();
            m_back.render();
        } break;

        case 2: {
            m_renderer.render(m_engine.c_getFEN(),
                              m_engine.c_getSelectedPiece(),
                              m_engine.c_getSelectedPieceMoves(),
                              m_engine.c_getOccupiedSquaresWhite(),
                              m_engine.c_getOccupiedSquaresBlack());
        } break;
    }

    EndDrawing();
}
