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
    // Piece() : color(PieceColor::White) {}
    Piece(PieceColor c, bool* isWhiteTurn_) : color(c), isWhiteTurn(isWhiteTurn_) {}

    PieceColor color;
    bool isWhiteTurn;

    // static Piece getPiece(char c);
    static const char* getImagePath(char c);

    virtual Bitboard generateMoves(Bitboard occupiedSquaresWhite, Bitboard occupiedSquaresBlack, Bitboard position) const { return 0; };

    virtual char getChar() const { return 0; };
};
