#pragma once


namespace Pieces
{
    constexpr enum class Piece : unsigned char {
        None,

        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King,

        PieceCount
    };

    constexpr char getPieceChar(Piece piece)
    {
        switch (piece) {
            case Piece::Pawn:   return 'p';
            case Piece::Knight: return 'n';
            case Piece::Bishop: return 'b';
            case Piece::Rook:   return 'r';
            case Piece::Queen:  return 'q';
            case Piece::King:   return 'k';

            default:            return '\0';
        }

        return '\0'; // Should not happen, if it does then you done goofed
    }
}
