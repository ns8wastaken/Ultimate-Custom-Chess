#pragma once
#include <cstdint>
#include <stdexcept>


typedef uint64_t Bitboard;


enum class PieceColor
{
    White,
    Black
};


class Piece
{
public:
    Piece() : color(PieceColor::White) {}
    Piece(PieceColor c) : color(c) {}

    PieceColor color;

    static Piece getPiece(char c);
    static const char* getImagePath(char c);

    virtual Bitboard generateMoves(Bitboard occupiedSquaresWhite, Bitboard occupiedSquaresBlack, int position) const { return 0; };

    virtual char getChar() const { return 0; };
};
