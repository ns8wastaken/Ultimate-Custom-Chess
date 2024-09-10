#include "engine.hpp"
#include "board.cpp"


Engine::Engine(const char* FEN)
    : m_board(FEN), m_requiresNewFEN(true)
{}


void Engine::update(const Vector2& mousePos)
{
    // Click location is of bounds
    if ((unsigned)mousePos.x >= Constants::ScreenSize || (unsigned)mousePos.y >= Constants::ScreenSize) {
        return;
    }


    m_selectedPieceMoves = generateMove(
        m_selectedPiecePos,
        m_selectedPieceType,
        m_board.occupiedSquaresWhite,
        m_board.occupiedSquaresBlack);


    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int clickRow_i = (int)(mousePos.x / Constants::SquareSize);
        int clickCol_i = (int)(mousePos.y / Constants::SquareSize);

        // Bitmask of clicked location
        uint64_t clickLocationBitmask = 1ULL << (clickCol_i * 8 + clickRow_i);

        bool pieceWasClicked = false;


        // If clicked square was part of the moves of the selected piece
        if (clickLocationBitmask & m_selectedPieceMoves) {
            m_board.makeMove(m_selectedPieceType, m_selectedPiecePos, clickLocationBitmask);
            m_requiresNewFEN = true;

            m_selectedPiecePos = 0ULL;
            m_selectedPieceType = Pieces::PieceType::None;
            m_selectedPieceMoves = 0ULL;
            return;
        }


        // If clicked square was the selected piece
        if (clickLocationBitmask & m_selectedPiecePos) {
            m_selectedPiecePos = 0ULL;
            m_selectedPieceType = Pieces::PieceType::None;
            m_selectedPieceMoves = 0ULL;
            return;
        }


        // Checks to see if piece was clicked
        for (int i = 0; i < PieceCount; ++i) {
            Pieces::PieceType pieceType = static_cast<Pieces::PieceType>(i);
            Bitboard bitboard = m_board.bitboards[i];

            // If piece was clicked
            if (bitboard & clickLocationBitmask) {
                pieceWasClicked = true;

                m_selectedPiecePos = clickLocationBitmask;
                m_selectedPieceType = pieceType;

                break;
            }
        }


        // This happens if no piece was clicked (empty square)
        if (!pieceWasClicked) {
            m_selectedPiecePos = 0ULL;
            m_selectedPieceType = Pieces::PieceType::None;
            m_selectedPieceMoves = 0ULL;
        }
    }
}


int Engine::evaluateBoard()
{
    return 0.0;
}


// std::vector<Bitboard> Engine::generateMoves() const
// {}


Bitboard Engine::generateMove(
    const Bitboard& position,
    const Pieces::PieceType& pieceType,
    const Bitboard& occupiedSquaresWhite,
    const Bitboard& occupiedSquaresBlack) const
{
    /*
        Bit bitmasks

            BitMaskA           BitMaskB
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B
        A * * * * * * *     * * * * * * * B

            BitMaskA2          BitMaskB2
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
        A A * * * * * *     * * * * * * B B
    */

    uint64_t BitMaskA = ~0x8080808080808080ULL;
    uint64_t BitMaskA2 = ~0xc0c0c0c0c0c0c0c0ULL;

    uint64_t BitMaskB = ~0x101010101010101ULL;
    uint64_t BitMaskB2 = ~0x303030303030303ULL;

    switch (pieceType) {
        case Pieces::PieceType::None:
        case Pieces::PieceType::PieceCount: return 0ULL;

        // Default pieces
        case Pieces::PieceType::PawnWhite: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, -8) & ~(occupiedSquaresWhite | occupiedSquaresBlack);
            moves |= Utils::BitShift(position, -7) & occupiedSquaresBlack;
            moves |= Utils::BitShift(position, -9) & occupiedSquaresBlack;

            return moves;
        }
        case Pieces::PieceType::PawnBlack: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, 8) & ~(occupiedSquaresWhite | occupiedSquaresBlack);
            moves |= Utils::BitShift(position, 7) & occupiedSquaresWhite;
            moves |= Utils::BitShift(position, 9) & occupiedSquaresWhite;

            return moves;
        }


        case Pieces::PieceType::KnightWhite:
        case Pieces::PieceType::KnightBlack: {
            Bitboard moves = m_board.precomputedMoves.knightMoves[__builtin_ctzll(position)];
            return moves & ~((pieceType == Pieces::PieceType::KnightWhite) ? occupiedSquaresWhite : occupiedSquaresBlack);
        }


        case Pieces::PieceType::BishopWhite:
        case Pieces::PieceType::BishopBlack: {
            int zeros = __builtin_ctzll(position);

            int distLeft = 7 - (zeros % 8);
            int distRight = zeros % 8;
            int distUp = 7 - (zeros / 8);
            int distDown = zeros / 8;

            int shifts[] = { 9, -9, 7, -7 };

            int maxLengths[] = {
                std::min(distLeft, distUp),    // Top left
                std::min(distRight, distDown), // Bottom right
                std::min(distRight, distUp),   // Top right
                std::min(distLeft, distDown)   // Bottom left
            };

            Bitboard moves = 0ULL;

            for (int i = 0; i < 4; ++i) {
                for (int j = 1; j <= maxLengths[i]; ++j) {
                    Bitboard result = Utils::BitShift(position, shifts[i] * j);
                    moves |= result;

                    if (result & (occupiedSquaresWhite | occupiedSquaresBlack))
                        break;
                }
            }

            return moves & ~((pieceType == Pieces::PieceType::BishopWhite) ? occupiedSquaresWhite : occupiedSquaresBlack);
        }


        case Pieces::PieceType::RookWhite:
        case Pieces::PieceType::RookBlack: {
            int zeros = __builtin_ctzll(position);

            int shifts[] = { 1, -1, 8, -8 };

            int maxLengths[] = {
                7 - (zeros % 8),
                zeros % 8,
                7 - (zeros / 8),
                zeros / 8
            };

            Bitboard moves = 0ULL;

            for (int i = 0; i < 4; ++i) {
                for (int j = 1; j <= maxLengths[i]; ++j) {
                    Bitboard result = Utils::BitShift(position, shifts[i] * j);
                    moves |= result;

                    if (result & (occupiedSquaresWhite | occupiedSquaresBlack))
                        break;
                }
            }

            return moves & ~((pieceType == Pieces::PieceType::RookWhite) ? occupiedSquaresWhite : occupiedSquaresBlack);
        }


        case Pieces::PieceType::QueenWhite:
        case Pieces::PieceType::QueenBlack: {
            int zeros = __builtin_ctzll(position);

            int distLeft = 7 - (zeros % 8);
            int distRight = zeros % 8;
            int distUp = 7 - (zeros / 8);
            int distDown = zeros / 8;

            int shifts[] = { 1, -1, 8, -8, 9, -9, 7, -7 };

            int maxLengths[] = {
                distLeft,
                distRight,
                distUp,
                distDown,
                std::min(distLeft, distUp),    // Top left
                std::min(distRight, distDown), // Bottom right
                std::min(distRight, distUp),   // Top right
                std::min(distLeft, distDown)   // Bottom left
            };

            Bitboard moves = 0ULL;

            for (int i = 0; i < 8; ++i) {
                for (int j = 1; j <= maxLengths[i]; ++j) {
                    Bitboard result = Utils::BitShift(position, shifts[i] * j);
                    moves |= result;

                    if (result & (occupiedSquaresWhite | occupiedSquaresBlack))
                        break;
                }
            }

            return moves & ~((pieceType == Pieces::PieceType::QueenWhite) ? occupiedSquaresWhite : occupiedSquaresBlack);
        }


        case Pieces::PieceType::KingWhite:
        case Pieces::PieceType::KingBlack: {
            Bitboard moves = m_board.precomputedMoves.kingMoves[__builtin_ctzll(position)];
            return moves & ~((pieceType == Pieces::PieceType::KingWhite) ? occupiedSquaresWhite : occupiedSquaresBlack);
        }



        // Custom pieces
        case Pieces::PieceType::CubistWhite:
        case Pieces::PieceType::CubistBlack: {
            Bitboard moves = m_board.precomputedMoves.cubistMoves[__builtin_ctzll(position)];
            return moves & ~((pieceType == Pieces::PieceType::CubistWhite) ? occupiedSquaresWhite : occupiedSquaresBlack);
        }


        case Pieces::PieceType::SnakeWhite: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, -8);
            moves |= Utils::BitShift(position, 1) & BitMaskB;
            moves |= Utils::BitShift(position, -1) & BitMaskA;

            moves |= Utils::BitShift(position, -16) & occupiedSquaresBlack;
            moves |= Utils::BitShift(position, 14) & BitMaskB2 & occupiedSquaresBlack;
            moves |= Utils::BitShift(position, 18) & BitMaskA2 & occupiedSquaresBlack;

            return moves & ~occupiedSquaresWhite;
        }
        case Pieces::PieceType::SnakeBlack: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, 8);
            moves |= Utils::BitShift(position, 1) & BitMaskB;
            moves |= Utils::BitShift(position, -1) & BitMaskA;

            moves |= Utils::BitShift(position, 16) & occupiedSquaresWhite;
            moves |= Utils::BitShift(position, -14) & BitMaskB2 & occupiedSquaresWhite;
            moves |= Utils::BitShift(position, -18) & BitMaskA2 & occupiedSquaresWhite;

            return moves & ~occupiedSquaresBlack;
        }
    }

    return 0ULL;
}


const Bitboard& Engine::c_getSelectedPiece()
{
    return m_selectedPiecePos;
}


const Bitboard& Engine::c_getSelectedPieceMoves()
{
    return m_selectedPieceMoves;
}


void Engine::m_generateFEN()
{
    m_FEN.clear();

    for (int col_i = 0; col_i < 8; ++col_i) {
        // Number of empty squares counter
        int emptyCount = 0;

        for (int row_i = 0; row_i < 8; ++row_i) {
            uint64_t bitLocationMask = 1ULL << (col_i * 8 + row_i);
            bool pieceFound = false;

            // Loop through all bitboards
            for (int i = 0; i < PieceCount; ++i) {
                Pieces::PieceType pieceType = static_cast<Pieces::PieceType>(i);
                Bitboard bitboard = m_board.bitboards[i];

                if (bitboard & bitLocationMask) {
                    if (emptyCount)
                        m_FEN += (char)(emptyCount + 48);

                    emptyCount = 0;
                    m_FEN += Pieces::getPieceChar(pieceType);

                    pieceFound = true;
                    break;
                }
            }

            if (!pieceFound) {
                ++emptyCount;
            }
        }

        if (emptyCount)
            m_FEN += (char)(emptyCount + 48);

        if (col_i < 7)
            m_FEN += '/';
    }
}


const char* Engine::c_getFEN()
{
    if (m_requiresNewFEN) {
        m_generateFEN();
        m_requiresNewFEN = false;
    }

    return m_FEN.c_str();
}



const Bitboard& Engine::c_getOccupiedSquaresWhite()
{
    return m_board.occupiedSquaresWhite;
}


const Bitboard& Engine::c_getOccupiedSquaresBlack()
{
    return m_board.occupiedSquaresBlack;
}
