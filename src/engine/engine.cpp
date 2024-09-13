#include "engine.hpp"
#include "board.cpp"


Engine::Engine(const char* FEN)
    : m_board(FEN), m_requiresNewFEN(true)
{}


void Engine::update(const Vector2& mousePos)
{
    if (m_isVsBot && !m_board.isWhiteTurn) {
        printf("Score: %i\n", negaMax(3));
        // printf("Score: %i\n", m_evaluateBoard());

        std::vector<Pieces::Move> moves = m_generateBotMoves();
        Pieces::Move move = moves[0];
        m_board.makeMove(m_board.pieceLookup[__builtin_ctzll(move.from)], move.from, move.to);
        m_requiresNewFEN = true;

        return;
    }

    // Click location is of bounds
    if ((unsigned)mousePos.x >= Constants::ScreenSize || (unsigned)mousePos.y >= Constants::ScreenSize) {
        return;
    }

    // Generate the selected piece's moves
    m_selectedPieceMoves = m_generateMove(m_selectedPiecePos, m_selectedPieceType, m_board.occupiedSquaresWhite, m_board.occupiedSquaresBlack);


    // For player(s)
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int clickRow_i = (int)(mousePos.x / Constants::SquareSize);
        int clickCol_i = (int)(mousePos.y / Constants::SquareSize);

        // Bitmask of clicked location
        uint64_t clickLocationBitmask = 1ULL << (clickCol_i * 8 + clickRow_i);


        // If clicked square was part of the moves of the selected piece
        if (clickLocationBitmask & m_selectedPieceMoves) {
            m_board.makeMove(m_selectedPieceType, m_selectedPiecePos, clickLocationBitmask);
            m_requiresNewFEN = true;

            m_selectedPiecePos = 0ULL;
            m_selectedPieceType = Pieces::PieceType::None;
            m_selectedPieceMoves = 0ULL;
            return;
        }


        // If clicked square was the already selected piece
        if (clickLocationBitmask & m_selectedPiecePos) {
            m_selectedPiecePos = 0ULL;
            m_selectedPieceType = Pieces::PieceType::None;
            m_selectedPieceMoves = 0ULL;
            return;
        }


        bool pieceWasClicked = false;

        // Checks to see if piece was clicked
        m_selectedPieceType = m_board.pieceLookup[__builtin_ctzll(clickLocationBitmask)];
        if (m_selectedPieceType != Pieces::PieceType::None &&
            ((m_board.isWhiteTurn && (m_selectedPieceType < Pieces::PieceType::WhitePiecesEnd)) ||
             (!m_board.isWhiteTurn && (m_selectedPieceType >= Pieces::PieceType::WhitePiecesEnd)))) {

            m_selectedPiecePos = clickLocationBitmask;
            pieceWasClicked = true;
        }


        // This happens if no piece was clicked (empty square)
        if (!pieceWasClicked) {
            m_selectedPiecePos = 0ULL;
            m_selectedPieceType = Pieces::PieceType::None;
            m_selectedPieceMoves = 0ULL;
        }
    }
}


int Engine::m_evaluateBoard()
{
    int score = 0;

    for (Pieces::PieceType pieceType : m_board.pieceLookup) {
        score += Pieces::getPieceValue(pieceType) * ((pieceType < Pieces::PieceType::WhitePiecesEnd) ? 1 : -1);
    }

    return score;
}


int Engine::quiescentSearch(int alpha, int beta)
{
    int eval = m_evaluateBoard();
    if (eval >= beta) return beta;

    if (alpha < eval) alpha = eval;

    for (Pieces::Move move : m_generateBotMoves()) {
        if (m_board.pieceLookup[__builtin_ctzll(move.to)] == Pieces::PieceType::None) continue;

        // Make move
        m_board.makeMove(m_board.pieceLookup[__builtin_ctzll(move.from)], move.from, move.to);

        int score = -quiescentSearch(-beta, -alpha);

        // Unmake move
        m_board.makeMove(m_board.pieceLookup[__builtin_ctzll(move.to)], move.to, move.from);

        if (score >= beta) return beta;

        if (score > alpha) alpha = score;
    }

    return alpha;
}


int Engine::negaMax(int depth)
{
    if (depth == 0) return -m_evaluateBoard();

    int max = -std::numeric_limits<int>::infinity();

    for (Pieces::Move move : m_generateBotMoves()) {
        // Make move
        m_board.makeMove(m_board.pieceLookup[__builtin_ctzll(move.from)], move.from, move.to);

        int score = -negaMax(depth - 1);

        // Unmake move
        m_board.makeMove(m_board.pieceLookup[__builtin_ctzll(move.to)], move.to, move.from);

        if (move.capturedPieceType != Pieces::PieceType::None) {
            m_board.bitboards[PieceToInt(move.capturedPieceType)] |= move.to;
            m_board.pieceLookup[__builtin_ctzll(move.to)] = move.capturedPieceType;

            if (move.capturedPieceType < Pieces::PieceType::WhitePiecesEnd) {
                m_board.occupiedSquaresWhite |= move.to;
            }
            else {
                m_board.occupiedSquaresBlack |= move.to;
            }
        }

        if (score > max) max = score;
    }

    return max;
}


Bitboard Engine::m_generateMove(
    const Bitboard& position,
    const Pieces::PieceType& pieceType,
    const Bitboard& occupiedSquaresWhite,
    const Bitboard& occupiedSquaresBlack) const
{
    const Bitboard occupiedSquaresAll = (occupiedSquaresWhite | occupiedSquaresBlack);

    switch (pieceType) {
        case Pieces::PieceType::None:
        case Pieces::PieceType::PieceCount: return 0ULL;


        // Default pieces
        case Pieces::PieceType::PawnWhite: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, -8) & ~occupiedSquaresAll;
            moves |= Utils::BitShift(position, -7) & occupiedSquaresBlack & Utils::BitMaskB;
            moves |= Utils::BitShift(position, -9) & occupiedSquaresBlack & Utils::BitMaskA;

            // Double push if possible
            if ((position & m_board.initialPawnsWhite) && (Utils::BitShift(position, -8) & ~occupiedSquaresAll))
                moves |= Utils::BitShift(position, -16) & ~occupiedSquaresAll;

            return moves;
        }
        case Pieces::PieceType::PawnBlack: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, 8) & ~occupiedSquaresAll;
            moves |= Utils::BitShift(position, 7) & occupiedSquaresWhite & Utils::BitMaskA;
            moves |= Utils::BitShift(position, 9) & occupiedSquaresWhite & Utils::BitMaskB;

            // Double push if possible
            if ((position & m_board.initialPawnsBlack) && (Utils::BitShift(position, 8) & ~occupiedSquaresAll))
                moves |= Utils::BitShift(position, 16) & ~occupiedSquaresAll;

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

                    if (result & occupiedSquaresAll)
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

                    if (result & occupiedSquaresAll)
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

                    if (result & occupiedSquaresAll)
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
            moves |= Utils::BitShift(position, 1) & Utils::BitMaskB;
            moves |= Utils::BitShift(position, -1) & Utils::BitMaskA;

            moves |= Utils::BitShift(position, -16) & occupiedSquaresBlack;
            moves |= Utils::BitShift(position, 14) & Utils::BitMaskB2 & occupiedSquaresBlack;
            moves |= Utils::BitShift(position, 18) & Utils::BitMaskA2 & occupiedSquaresBlack;

            return moves & ~occupiedSquaresWhite;
        }
        case Pieces::PieceType::SnakeBlack: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, 8);
            moves |= Utils::BitShift(position, 1) & Utils::BitMaskB;
            moves |= Utils::BitShift(position, -1) & Utils::BitMaskA;

            moves |= Utils::BitShift(position, 16) & occupiedSquaresWhite;
            moves |= Utils::BitShift(position, -14) & Utils::BitMaskB2 & occupiedSquaresWhite;
            moves |= Utils::BitShift(position, -18) & Utils::BitMaskA2 & occupiedSquaresWhite;

            return moves & ~occupiedSquaresBlack;
        }


        case Pieces::PieceType::SoldierWhite: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, -8) & ~occupiedSquaresAll;
            moves |= Utils::BitShift(position, -7) & occupiedSquaresBlack;
            moves |= Utils::BitShift(position, -9) & occupiedSquaresBlack;

            // Double push if possible
            if ((position & m_board.initialPawnsWhite) && (Utils::BitShift(position, -8) & ~occupiedSquaresAll))
                moves |= Utils::BitShift(position, -16) & ~occupiedSquaresAll;

            Bitboard otherSoldiers = m_board.bitboards[PieceToInt(pieceType)] & ~position;

            int zeros = __builtin_ctzll(otherSoldiers);
            while (zeros < 64) {
                moves |= Utils::BitShift(1ULL << zeros, 1) & Utils::BitMaskB & ~occupiedSquaresAll;
                moves |= Utils::BitShift(1ULL << zeros, -1) & Utils::BitMaskA & ~occupiedSquaresAll;


                otherSoldiers ^= (1ULL << zeros);
                zeros = __builtin_ctzll(otherSoldiers);
            }

            return moves;
        }
        case Pieces::PieceType::SoldierBlack: {
            Bitboard moves = 0ULL;

            moves |= Utils::BitShift(position, 8) & ~occupiedSquaresAll;
            moves |= Utils::BitShift(position, 7) & occupiedSquaresWhite;
            moves |= Utils::BitShift(position, 9) & occupiedSquaresWhite;

            // Double push if possible
            if ((position & m_board.initialPawnsBlack) && (Utils::BitShift(position, 8) & ~occupiedSquaresAll))
                moves |= Utils::BitShift(position, 16) & ~occupiedSquaresAll;

            Bitboard otherSoldiers = m_board.bitboards[PieceToInt(pieceType)] & ~position;

            int zeros = __builtin_ctzll(otherSoldiers);
            while (zeros < 64) {
                moves |= Utils::BitShift(1ULL << zeros, 1) & Utils::BitMaskB & ~occupiedSquaresAll;
                moves |= Utils::BitShift(1ULL << zeros, -1) & Utils::BitMaskA & ~occupiedSquaresAll;


                otherSoldiers ^= (1ULL << zeros);
                zeros = __builtin_ctzll(otherSoldiers);
            }

            return moves;
        }


        case Pieces::PieceType::ObserverWhite: {
            Bitboard moves = m_board.precomputedMoves.kingMoves[__builtin_ctzll(position)] & ~occupiedSquaresWhite;

            Bitboard kingPos = m_board.bitboards[PieceToInt(Pieces::PieceType::KingWhite)];

            return moves | (m_board.precomputedMoves.kingMoves[__builtin_ctzll(kingPos)] & ~occupiedSquaresWhite);
        }
        case Pieces::PieceType::ObserverBlack: {
            Bitboard moves = m_board.precomputedMoves.kingMoves[__builtin_ctzll(position)] & ~occupiedSquaresBlack;

            Bitboard kingPos = m_board.bitboards[PieceToInt(Pieces::PieceType::KingBlack)];

            return moves | (m_board.precomputedMoves.kingMoves[__builtin_ctzll(kingPos)] & ~occupiedSquaresBlack);
        }


        case Pieces::PieceType::FoolWhite:
        case Pieces::PieceType::FoolBlack: {
            Bitboard moves = m_board.precomputedMoves.foolMoves[__builtin_ctzll(position)];

            Bitboard upPos = Utils::BitShift(position, -8);
            Bitboard downPos = Utils::BitShift(position, 8);
            Bitboard leftPos = Utils::BitShift(position, 1);
            Bitboard rightPos = Utils::BitShift(position, -1);

            Bitboard occupiedSquaresAllExceptSelf = occupiedSquaresAll & ~position;

            Bitboard neighbors = 0ULL;

            if (moves & upPos) {
                neighbors |= Utils::BitShift(upPos, 1);
                neighbors |= Utils::BitShift(upPos, -1);
                neighbors |= Utils::BitShift(upPos, 8);
                neighbors |= Utils::BitShift(upPos, -8);

                if (neighbors & occupiedSquaresAllExceptSelf)
                    moves |= m_board.precomputedMoves.foolMoves[__builtin_ctzll(upPos)];
            }
            neighbors = 0ULL;

            if (moves & downPos) {
                neighbors |= Utils::BitShift(downPos, 1);
                neighbors |= Utils::BitShift(downPos, -1);
                neighbors |= Utils::BitShift(downPos, 8);
                neighbors |= Utils::BitShift(downPos, -8);

                if (neighbors & occupiedSquaresAllExceptSelf)
                    moves |= m_board.precomputedMoves.foolMoves[__builtin_ctzll(downPos)];
            }
            neighbors = 0ULL;

            if (moves & leftPos) {
                neighbors |= Utils::BitShift(leftPos, 1);
                neighbors |= Utils::BitShift(leftPos, -1);
                neighbors |= Utils::BitShift(leftPos, 8);
                neighbors |= Utils::BitShift(leftPos, -8);

                if (neighbors & occupiedSquaresAllExceptSelf)
                    moves |= m_board.precomputedMoves.foolMoves[__builtin_ctzll(leftPos)];
            }
            neighbors = 0ULL;

            if (moves & rightPos) {
                neighbors |= Utils::BitShift(rightPos, 1);
                neighbors |= Utils::BitShift(rightPos, -1);
                neighbors |= Utils::BitShift(rightPos, 8);
                neighbors |= Utils::BitShift(rightPos, -8);

                if (neighbors & occupiedSquaresAllExceptSelf)
                    moves |= m_board.precomputedMoves.foolMoves[__builtin_ctzll(rightPos)];
            }


            return moves & ~((pieceType == Pieces::PieceType::FoolWhite) ? occupiedSquaresWhite : occupiedSquaresBlack);
        }


        case Pieces::PieceType::GodWhite:
        case Pieces::PieceType::GodBlack: {
            Bitboard moves = m_board.precomputedMoves.godMoves[__builtin_ctzll(position)];
            return moves & ~((pieceType == Pieces::PieceType::GodWhite) ? occupiedSquaresWhite : occupiedSquaresBlack);
        }
    }

    return 0ULL;
}


std::vector<Pieces::Move> Engine::m_generateBotMoves() const
{
    std::vector<Pieces::Move> botMoves;

    for (size_t i = 0; i < m_board.pieceLookup.size(); ++i) {
        Pieces::PieceType pieceType = m_board.pieceLookup[i];

        if ((pieceType == Pieces::PieceType::None) ||
            (pieceType < Pieces::PieceType::WhitePiecesEnd)) {
            continue;
        }


        Bitboard position = 1ULL << i;

        Bitboard pieceMoves = m_generateMove(position, pieceType, m_board.occupiedSquaresWhite, m_board.occupiedSquaresBlack);

        int offset = 0;
        while (pieceMoves) {
            if (pieceMoves & 0x1) {
                Bitboard newPos = 1ULL << offset;
                botMoves.push_back(Pieces::Move{ m_board.pieceLookup[__builtin_ctzll(newPos)], position, newPos });
            }
            ++offset;
            pieceMoves >>= 1;
        }
    }

    return botMoves;
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


void Engine::setGamemode(bool isVsBot)
{
    m_isVsBot = isVsBot;
}


const Bitboard& Engine::c_getOccupiedSquaresWhite()
{
    return m_board.occupiedSquaresWhite;
}


const Bitboard& Engine::c_getOccupiedSquaresBlack()
{
    return m_board.occupiedSquaresBlack;
}
