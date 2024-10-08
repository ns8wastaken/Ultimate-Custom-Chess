#include "board.hpp"


Board::Board(const char* FEN)
{
    pieceLookup.fill(Pieces::PieceType::None);

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


void Board::makeMove(const Pieces::PieceType pieceType, const Bitboard& from, const Bitboard& to, bool addToHistory)
{
    if (addToHistory) {
        m_moveHistory.push(Pieces::HistoryMove{pieceLookup[__builtin_ctzll(to)], from, to});
    }

    isWhiteTurn = !isWhiteTurn;
    depthPly += 1;
    depthMove += !(depthPly % 2);


    // If en passant is active
    if (enPassant.from) {
        Pieces::PieceType enPassantPieceType = pieceLookup[__builtin_ctzll(enPassant.to)];

        // Check if en passant square was attacked
        if ((pieceType == Pieces::PieceType::PawnWhite ||
             pieceType == Pieces::PieceType::PawnBlack) &&
            to == enPassant.from) {
            bitboards[PieceToInt(enPassantPieceType)] &= ~enPassant.to;
            occupiedSquaresWhite &= ~enPassant.to;
            occupiedSquaresBlack &= ~enPassant.to;
        }

        // Remove hitbox
        // Occupied square will later be set to 1 for the piece's "to" location (or en passant's from location)
        occupiedSquaresWhite &= ~enPassant.from;
        occupiedSquaresBlack &= ~enPassant.from;


        // Reset en passant
        enPassant.from = 0ULL;
        enPassant.to = 0ULL;
    }


    // If the "to" square has a piece on it
    Pieces::PieceType toPieceType = pieceLookup[__builtin_ctzll(to)];
    if (toPieceType != Pieces::PieceType::None) {
        // Remove killed piece from bitboards
        bitboards[PieceToInt(toPieceType)] &= ~to;

        // Remove killed piece from occupied squares
        if (toPieceType < Pieces::PieceType::WhitePiecesEnd) {
            occupiedSquaresWhite &= ~to;
        }
        else {
            occupiedSquaresBlack &= ~to;
        }
    }


    // Move piece
    bitboards[PieceToInt(pieceType)] &= ~from;
    bitboards[PieceToInt(pieceType)] |= to;

    pieceLookup[__builtin_ctzll(from)] = Pieces::PieceType::None;
    pieceLookup[__builtin_ctzll(to)] = pieceType;


    // Set en passant square if a pawn was moved 2 squares
    if ((pieceType == Pieces::PieceType::PawnWhite) && (to == (from >> 16))) [[unlikely]] {
        enPassant.from = from >> 8;
        enPassant.to = to;
        occupiedSquaresWhite |= enPassant.from;
    }
    else if ((pieceType == Pieces::PieceType::PawnBlack) && (to == (from << 16))) [[unlikely]] {
        enPassant.from = from << 8;
        enPassant.to = to;
        occupiedSquaresBlack |= enPassant.from;
    }


    // Update occupied squares
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
    precomputedMoves.foolMoves.fill(0ULL);


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


        // Fool
        {
            precomputedMoves.foolMoves[i] |= Utils::BitShift(1ULL << i, 8);
            precomputedMoves.foolMoves[i] |= Utils::BitShift(1ULL << i, 1) & Utils::BitMaskB;
            precomputedMoves.foolMoves[i] |= Utils::BitShift(1ULL << i, -1) & Utils::BitMaskA;
            precomputedMoves.foolMoves[i] |= Utils::BitShift(1ULL << i, -8);
        }


        // God
        {
            precomputedMoves.godMoves[i] = precomputedMoves.kingMoves[i] | precomputedMoves.cubistMoves[i];
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
                    Pieces::PieceType pieceType = PieceFromChar(Char);
                    bitboards[PieceToInt(pieceType)] |= 1ULL << (col_i * 8 + row_i);
                    pieceLookup[col_i * 8 + row_i] = pieceType;
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


void Board::undoMove()
{
    Pieces::HistoryMove move = m_moveHistory.top();
    m_moveHistory.pop();

    makeMove(pieceLookup[__builtin_ctzll(move.to)], move.to, move.from, false);

    if (move.capturedPieceType == Pieces::PieceType::None) return;

    bitboards[PieceToInt(move.capturedPieceType)] |= move.to;
    pieceLookup[__builtin_ctzll(move.to)] = move.capturedPieceType;

    if (move.capturedPieceType < Pieces::PieceType::WhitePiecesEnd)
        occupiedSquaresWhite |= move.to;
    else
        occupiedSquaresBlack |= move.to;
}
