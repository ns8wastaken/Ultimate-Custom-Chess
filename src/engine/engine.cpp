#include "engine.hpp"
#include "board.cpp"


Engine::Engine()
    : m_board(Board("kkKKKk2/kkKK"))
{}


int Engine::evaluateBoard()
{
    return 0.0;
}


const std::unordered_map<char, Bitboard>& Engine::getBitboard()
{
    return m_board.bitboards;
}
