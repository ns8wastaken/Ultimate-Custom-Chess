#include "engine.hpp"
#include "board.cpp"


Engine::Engine(const char* FEN)
    : m_board(Board(FEN))
{}


void Engine::update(Vector2 mousePos)
{
}


int Engine::evaluateBoard()
{
    return 0.0;
}


const BitboardArray& Engine::c_getBitboards()
{
    return m_board.bitboards;
}


const char* Engine::c_getFEN()
{
    return m_board.generateFEN();
}
