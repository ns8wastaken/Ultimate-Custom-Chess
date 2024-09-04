#include "board.hpp"


Board::Board()
{
    assertPieceChars();
}


Board::Board(const char* FEN)
{
    assertPieceChars();
}


void Board::assertPieceChars()
{
    for (Piece piece = Piece::None; piece != Piece::PieceCount; piece = static_cast<Piece>(static_cast<unsigned char>(piece) + 1)) {
        if (Pieces::getPieceChar(piece) == '\0')
            std::__throw_runtime_error("Not all of the pieces have a char defined.");
    }
}
