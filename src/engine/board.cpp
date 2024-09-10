#include "board.hpp"


Board::Board(const char* FEN)
{
    m_precomputeMoves();
    m_loadFEN(FEN);

    for (int i = PieceToInt(Pieces::PieceType::WhitePiecesStart); i < PieceToInt(Pieces::PieceType::WhitePiecesEnd); ++i) {
        occupiedSquaresWhite |= bitboards[i];

        // Get the initial position of white pawns
        if (static_cast<Pieces::PieceType>(i) == Pieces::PieceType::PawnWhite)
            initialPawnsWhite = bitboards[i];
    }

    for (int i = PieceToInt(Pieces::PieceType::BlackPiecesStart); i < PieceToInt(Pieces::PieceType::BlackPiecesEnd); ++i) {
        occupiedSquaresBlack |= bitboards[i];

        // Get the initial position of black pawns
        if (static_cast<Pieces::PieceType>(i) == Pieces::PieceType::PawnBlack)
            initialPawnsBlack = bitboards[i];
    }
}


void Board::makeMove(const Pieces::PieceType& pieceType, const Bitboard& from, const Bitboard& to)
{
    // Loop through all other pieces
    for (int i = 0; i < PieceToInt(Pieces::PieceType::PieceCount); ++i) {
        // If there is a piece in the "from" square
        if (to & bitboards[i]) {
            bitboards[i] &= ~to;

            // Update the occupied squares
            if (i < PieceToInt(Pieces::PieceType::WhitePiecesEnd)) {
                occupiedSquaresWhite &= ~to;
            }
            else {
                occupiedSquaresBlack &= ~to;
            }

            break;
        }
    }


    // Move piece
    bitboards[PieceToInt(pieceType)] &= ~from;
    bitboards[PieceToInt(pieceType)] |= to;

    if (pieceType < Pieces::PieceType::WhitePiecesEnd) {
        occupiedSquaresWhite &= ~from;
        occupiedSquaresWhite |= to;
        occupiedSquaresBlack &= ~to;
    }
    else {
        occupiedSquaresBlack &= ~from;
        occupiedSquaresBlack |= to;
        occupiedSquaresWhite &= ~to;
    }
}


void Board::m_precomputeMoves()
{
    precomputedMoves.knightMoves.fill(0ULL);
    precomputedMoves.kingMoves.fill(0ULL);

    precomputedMoves.cubistMoves.fill(0ULL);


    for (uint64_t i = 0; i < 64; ++i) {
        // Knight
        {
            precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, 17) & Utils::BitMaskB;
            precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, 15) & Utils::BitMaskA;
            precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, 10) & Utils::BitMaskB2;
            precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, 6) & Utils::BitMaskA2;
            precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, -6) & Utils::BitMaskB2;
            precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, -10) & Utils::BitMaskA2;
            precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, -15) & Utils::BitMaskB;
            precomputedMoves.knightMoves[i] |= Utils::BitShift(1ULL << i, -17) & Utils::BitMaskA;
        }


        // King
        {
            precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, 9) & Utils::BitMaskB;
            precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, 8);
            precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, 7) & Utils::BitMaskA;
            precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, 1) & Utils::BitMaskB;
            precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, -1) & Utils::BitMaskA;
            precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, -7) & Utils::BitMaskB;
            precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, -8);
            precomputedMoves.kingMoves[i] |= Utils::BitShift(1ULL << i, -9) & Utils::BitMaskA;
        }


        // Cubist
        {
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 18) & Utils::BitMaskB2;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 10) & Utils::BitMaskB2;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 2) & Utils::BitMaskB2;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -6) & Utils::BitMaskB2;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -14) & Utils::BitMaskB2;

            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 14) & Utils::BitMaskA2;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 6) & Utils::BitMaskA2;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -2) & Utils::BitMaskA2;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -10) & Utils::BitMaskA2;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -18) & Utils::BitMaskA2;

            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 17) & Utils::BitMaskB;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 15) & Utils::BitMaskA;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -15) & Utils::BitMaskB;
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -17) & Utils::BitMaskA;

            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, 16);
            precomputedMoves.cubistMoves[i] |= Utils::BitShift(1ULL << i, -16);
        }
    }
}


void Board::m_loadFEN(const char* FEN)
{
    bitboards.fill(0ULL);

    int row_i = 0;
    int col_i = 0;

    int i = 1;
    int step = 0;
    for (char Char = *FEN; Char != '\0'; Char = *(FEN + i), ++i) {
        switch (step) {
            case 0: {
                if (std::isdigit(Char)) {
                    row_i += (int)Char - (int)'0';
                }
                else if (Char == '/') {
                    row_i = 0;
                    ++col_i;
                }
                else if (Char == ' ') {
                    ++step;
                }
                else {
                    bitboards[PieceToInt(PieceFromChar(Char))] |= 1ULL << (col_i * 8 + row_i);
                    ++row_i;
                }
                break;
            }

            case 1: {
                if (Char == 'w')
                    isWhiteTurn = true;
                else if (Char == 'b')
                    isWhiteTurn = false;
                ++step;
                break;
            }
        }
    }
}
