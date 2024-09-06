#include "piece.hpp"

#include "king.hpp"


// Piece Piece::getPiece(char c)
// {
//     switch (c) {
//         case 'K': return King(PieceColor::White);
//         case 'k': return King(PieceColor::Black);
//     }

//     std::__throw_runtime_error("A char does not have a piece assigned.");
// }


const char* Piece::getImagePath(char c)
{
    switch (c) {
        case 'K': return "src/interface/assets/pieces/king/king_white.png";
        case 'k': return "src/interface/assets/pieces/king/king_black.png";
        case 'Q': return "src/interface/assets/pieces/queen/queen_white.png";
        case 'q': return "src/interface/assets/pieces/queen/queen_black.png";
    }

    std::__throw_runtime_error("A loaded piece does not have a texture file path assigned.");
}
