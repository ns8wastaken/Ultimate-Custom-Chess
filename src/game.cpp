#include "game.hpp"
#include "engine/engine.cpp"
#include "interface/board_renderer.cpp"


Game::Game()
    : m_engine("rnbqkbnr/pppfpppp/8/8/8/8/PPPPFPPP/RNBQKBNR w")
{}


void Game::update()
{
    switch (m_mode) {
        case 0: {
            break;
        }

        case 2: m_engine.update(GetMousePosition()); break;
    }
}


void Game::render()
{
    BeginDrawing();

    switch (m_mode) {
        case 0: {
        }

        case 2: {
            m_renderer.render(m_engine.c_getFEN(),
                              m_engine.c_getSelectedPiece(),
                              m_engine.c_getSelectedPieceMoves(),
                              m_engine.c_getOccupiedSquaresWhite(),
                              m_engine.c_getOccupiedSquaresBlack());
            break;
        }
    }

    EndDrawing();
}
