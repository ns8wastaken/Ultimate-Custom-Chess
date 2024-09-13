#pragma once


#define PieceToInt(x) static_cast<int>(x)
#define PieceFromChar(x) Pieces::getPieceTypeFromChar(x)


namespace Pieces
{

    enum class PieceType
    {
        // White pieces
        PawnWhite,
        KnightWhite,
        BishopWhite,
        RookWhite,
        QueenWhite,
        KingWhite,

        CubistWhite,
        SnakeWhite,
        SoldierWhite,
        ObserverWhite,
        FoolWhite,
        GodWhite,



        // Black pieces
        PawnBlack,
        KnightBlack,
        BishopBlack,
        RookBlack,
        QueenBlack,
        KingBlack,

        CubistBlack,
        SnakeBlack,
        SoldierBlack,
        ObserverBlack,
        FoolBlack,
        GodBlack,



        // Utils
        PieceCount,
        None,

        WhitePiecesStart = 0,
        WhitePiecesEnd = (int)(PieceCount / 2),
        BlackPiecesStart = WhitePiecesEnd,
        BlackPiecesEnd = PieceCount
    };


    struct Move
    {
        uint64_t from = 0ULL;
        uint64_t to = 0ULL;
    };


    constexpr int getPieceValue(PieceType type)
    {
        switch (type) {
            case PieceType::None:
            case PieceType::PieceCount: return 0;

            // Default pieces
            case PieceType::PawnWhite:
            case PieceType::PawnBlack:   return 100;

            case PieceType::KnightWhite:
            case PieceType::KnightBlack: return 300;

            case PieceType::BishopWhite:
            case PieceType::BishopBlack: return 300;

            case PieceType::RookWhite:
            case PieceType::RookBlack:   return 500;

            case PieceType::QueenWhite:
            case PieceType::QueenBlack:  return 900;

            case PieceType::KingWhite:
            case PieceType::KingBlack:   return 314159265;



            // Custom pieces
            case PieceType::CubistWhite:
            case PieceType::CubistBlack:   return 650;

            case PieceType::SnakeWhite:
            case PieceType::SnakeBlack:    return 200;

            case PieceType::SoldierWhite:
            case PieceType::SoldierBlack:  return 200;

            case PieceType::ObserverWhite:
            case PieceType::ObserverBlack: return 350;

            case PieceType::FoolWhite:
            case PieceType::FoolBlack:     return 250;

            case PieceType::GodWhite:
            case PieceType::GodBlack:      return 1000;
        }
    }


    constexpr char getPieceChar(PieceType type)
    {
        switch (type) {
            case PieceType::None:
            case PieceType::PieceCount: return '\0';

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
            case PieceType::CubistWhite:   return 'C';
            case PieceType::CubistBlack:   return 'c';

            case PieceType::SnakeWhite:    return 'S';
            case PieceType::SnakeBlack:    return 's';

            case PieceType::SoldierWhite:  return 'W';
            case PieceType::SoldierBlack:  return 'w';

            case PieceType::ObserverWhite: return 'O';
            case PieceType::ObserverBlack: return 'o';

            case PieceType::FoolWhite:     return 'F';
            case PieceType::FoolBlack:     return 'f';

            case PieceType::GodWhite:      return 'G';
            case PieceType::GodBlack:      return 'g';
        }

        return '\0';
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
            case 'C': return PieceType::CubistWhite;
            case 'c': return PieceType::CubistBlack;

            case 'S': return PieceType::SnakeWhite;
            case 's': return PieceType::SnakeBlack;

            case 'W': return PieceType::SoldierWhite;
            case 'w': return PieceType::SoldierBlack;

            case 'O': return PieceType::ObserverWhite;
            case 'o': return PieceType::ObserverBlack;

            case 'F': return PieceType::FoolWhite;
            case 'f': return PieceType::FoolBlack;

            case 'G': return PieceType::GodWhite;
            case 'g': return PieceType::GodBlack;
        }

        return PieceType::None;
    };


    constexpr const char* getPieceSpritePath(PieceType type)
    {
        switch (type) {
            case PieceType::None:
            case PieceType::PieceCount: return "";

            // White default pieces
            case PieceType::PawnWhite:   return "src/assets/pieces/pawn/pawn_white.png";
            case PieceType::KnightWhite: return "src/assets/pieces/knight/knight_white.png";
            case PieceType::BishopWhite: return "src/assets/pieces/bishop/bishop_white.png";
            case PieceType::RookWhite:   return "src/assets/pieces/rook/rook_white.png";
            case PieceType::QueenWhite:  return "src/assets/pieces/queen/queen_white.png";
            case PieceType::KingWhite:   return "src/assets/pieces/king/king_white.png";

            // Black default pieces
            case PieceType::PawnBlack:   return "src/assets/pieces/pawn/pawn_black.png";
            case PieceType::KnightBlack: return "src/assets/pieces/knight/knight_black.png";
            case PieceType::BishopBlack: return "src/assets/pieces/bishop/bishop_black.png";
            case PieceType::RookBlack:   return "src/assets/pieces/rook/rook_black.png";
            case PieceType::QueenBlack:  return "src/assets/pieces/queen/queen_black.png";
            case PieceType::KingBlack:   return "src/assets/pieces/king/king_black.png";



            // Custom pieces
            case PieceType::CubistWhite:   return "src/assets/pieces/cubist/cubist_white.png";
            case PieceType::CubistBlack:   return "src/assets/pieces/cubist/cubist_black.png";

            case PieceType::SnakeWhite:    return "src/assets/pieces/snake/snake_white.png";
            case PieceType::SnakeBlack:    return "src/assets/pieces/snake/snake_black.png";

            case PieceType::SoldierWhite:  return "src/assets/pieces/soldier/soldier_white.png";
            case PieceType::SoldierBlack:  return "src/assets/pieces/soldier/soldier_black.png";

            case PieceType::ObserverWhite: return "src/assets/pieces/observer/observer_white.png";
            case PieceType::ObserverBlack: return "src/assets/pieces/observer/observer_black.png";

            case PieceType::FoolWhite:     return "src/assets/pieces/fool/fool_white.png";
            case PieceType::FoolBlack:     return "src/assets/pieces/fool/fool_black.png";

            case PieceType::GodWhite:      return "src/assets/pieces/god/god_white.png";
            case PieceType::GodBlack:      return "src/assets/pieces/god/god_black.png";
        }

        return "";
    };


    constexpr float getPieceSizeMult(char c)
    {
        switch (c) {
            // White default pieces
            case 'P': return 1.0f;
            case 'N': return 1.0f;
            case 'B': return 1.0f;
            case 'R': return 1.0f;
            case 'Q': return 1.0f;
            case 'K': return 1.0f;

            // Black default pieces
            case 'p': return 1.0f;
            case 'n': return 1.0f;
            case 'b': return 1.0f;
            case 'r': return 1.0f;
            case 'q': return 1.0f;
            case 'k': return 1.0f;



            // Custom pieces
            case 'C': return 1.0f;
            case 'c': return 1.0f;

            case 'S': return 1.0f;
            case 's': return 1.0f;

            case 'W': return 1.0f;
            case 'w': return 1.0f;

            case 'O': return 1.0f;
            case 'o': return 1.0f;

            case 'F': return 1.0f;
            case 'f': return 1.0f;

            case 'G': return 1.0f;
            case 'g': return 1.0f;
        }

        return 0.0f;
    };

}
