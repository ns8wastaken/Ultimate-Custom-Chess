#include "piece.hpp"

#include "king.hpp"


Piece Piece::getPiece(char c)
{
    switch (c) {
        case 'K': return King(PieceColor::White);
        case 'k': return King(PieceColor::Black);
    }

    return Piece();
}


const char* Piece::getImagePath(char c)
{
    switch (c) {
        case 'K': return "src/interface/assets/pieces/king/king_white.png";
        case 'k': return "src/interface/assets/pieces/king/king_black.png";
    }

    std::__throw_runtime_error("A loaded piece does not have a texture file path assigned.");
}
