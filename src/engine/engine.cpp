#include "engine.hpp"
#include "board.cpp"


Engine::Engine()
    : m_board(Board("qQQqq/kQKq"))
{}


void Engine::update(Vector2 mousePos)
{
}


int Engine::evaluateBoard()
{
    return 0.0;
}


const std::unordered_map<char, Bitboard>& Engine::c_getBitboard()
{
    return m_board.bitboards;
}
