#pragma once


#define PieceToInt(x) static_cast<int>(x)
#define PieceFromChar(x) Pieces::getPieceTypeFromChar(x)


namespace Pieces
{

    // clang-format off
    enum class PieceType
    {
        // White default pieces
        PawnWhite,
        KnightWhite,
        BishopWhite,
        RookWhite,
        QueenWhite,
        KingWhite,

        // Black default pieces
        PawnBlack,
        KnightBlack,
        BishopBlack,
        RookBlack,
        QueenBlack,
        KingBlack,

        // Number of pieces
        PieceCount
    };
    // clang-format on


    constexpr char getPieceChar(PieceType type)
    {
        switch (type) {
            case PieceType::PieceCount:  return '\0';

            // White default pieces
            case PieceType::PawnWhite:   return 'P';
            case PieceType::KnightWhite: return 'N';
            case PieceType::BishopWhite: return 'B';
            case PieceType::RookWhite:   return 'R';
            case PieceType::QueenWhite:  return 'Q';
            case PieceType::KingWhite:   return 'K';

            // Black default pieces
            case PieceType::PawnBlack:   return 'p';
            case PieceType::KnightBlack: return 'n';
            case PieceType::BishopBlack: return 'b';
            case PieceType::RookBlack:   return 'r';
            case PieceType::QueenBlack:  return 'q';
            case PieceType::KingBlack:   return 'k';

            // Custom pieces
        }
    };


    constexpr PieceType getPieceTypeFromChar(char c)
    {
        switch (c) {
            // White default pieces
            case 'P': return PieceType::PawnWhite;
            case 'N': return PieceType::KnightWhite;
            case 'B': return PieceType::BishopWhite;
            case 'R': return PieceType::RookWhite;
            case 'Q': return PieceType::QueenWhite;
            case 'K': return PieceType::KingWhite;

            // Black default pieces
            case 'p': return PieceType::PawnBlack;
            case 'n': return PieceType::KnightBlack;
            case 'b': return PieceType::BishopBlack;
            case 'r': return PieceType::RookBlack;
            case 'q': return PieceType::QueenBlack;
            case 'k': return PieceType::KingBlack;

            // Custom pieces
        }
    };


    const char* getPieceSpritePath(PieceType type)
    {
        switch (type) {
            case PieceType::PieceCount:  return "";

            // White default pieces
            case PieceType::PawnWhite:   return "src/interface/assets/pieces/pawn/pawn_white.png";
            case PieceType::KnightWhite: return "src/interface/assets/pieces/knight/knight_white.png";
            case PieceType::BishopWhite: return "src/interface/assets/pieces/bishop/bishop_white.png";
            case PieceType::RookWhite:   return "src/interface/assets/pieces/rook/rook_white.png";
            case PieceType::QueenWhite:  return "src/interface/assets/pieces/queen/queen_white.png";
            case PieceType::KingWhite:   return "src/interface/assets/pieces/king/king_white.png";

            // Black default pieces
            case PieceType::PawnBlack:   return "src/interface/assets/pieces/pawn/pawn_black.png";
            case PieceType::KnightBlack: return "src/interface/assets/pieces/knight/knight_black.png";
            case PieceType::BishopBlack: return "src/interface/assets/pieces/bishop/bishop_black.png";
            case PieceType::RookBlack:   return "src/interface/assets/pieces/rook/rook_black.png";
            case PieceType::QueenBlack:  return "src/interface/assets/pieces/queen/queen_black.png";
            case PieceType::KingBlack:
                return "src/interface/assets/pieces/king/king_black.png";

            // Custom pieces
        }
    };

}
