#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>

class ChessBoard;

enum class PieceType {
    NONE,
    ROOK,
    KNIGHT,
    BISHOP,
    KING,
    QUEEN,
    PAWN
};

enum class PieceColor {
    NONE,
    WHITE,
    BLACK
};

class Piece {
protected:
    PieceType type;
    PieceColor color;
    bool hasMoved;

public:
    Piece() : type(PieceType::NONE), color(PieceColor::NONE), hasMoved(false) {}
    Piece(PieceType type, PieceColor color) : type(type), color(color), hasMoved(false) {}
    virtual ~Piece() = default;

    PieceType getType() const { return type; }
    PieceColor getColor() const { return color; }
    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    bool isWhite() const { return color == PieceColor::WHITE; }
    bool isBlack() const { return color == PieceColor::BLACK; }
    bool isEmpty() const { return type == PieceType::NONE; }

    // Konwersja figury na wartoœæ liczbow¹ (dla kompatybilnoœci)
    int toInt() const {
        int value = 0;

        switch (type) {
        case PieceType::ROOK:   value = 1; break;
        case PieceType::KNIGHT: value = 2; break;
        case PieceType::BISHOP: value = 3; break;
        case PieceType::KING:   value = 4; break;
        case PieceType::QUEEN:  value = 5; break;
        case PieceType::PAWN:   value = 6; break;
        default:                value = 0; break;
        }

        return isWhite() ? value : -value;
    }

    virtual std::vector<sf::Vector2i> getPossibleMoves(int row, int col, const ChessBoard& board) const = 0;
};

class EmptyPiece : public Piece {
public:
    EmptyPiece() : Piece(PieceType::NONE, PieceColor::NONE) {}
    std::vector<sf::Vector2i> getPossibleMoves(int row, int col, const ChessBoard& board) const override {
        return {}; // Puste pole nie ma ruchów
    }
};

// Forward declarations dla klas figur
class Pawn;
class Rook;
class Knight;
class Bishop;
class Queen;
class King;