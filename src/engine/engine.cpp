#include "engine.hpp"
#include "board.cpp"


Engine::Engine()
    : m_board(Board("kkKKKk"))
{}


int Engine::evaluateBoard()
{
    return 0.0;
}


const Board& Engine::getBoard()
{
    return m_board;
}
