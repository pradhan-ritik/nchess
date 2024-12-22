#ifndef PIECE_HPP
#define PIECE_HPP

#include "constants.hpp"

enum PIECE : uint8_t {
    EMPTY,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

#endif // PIECE_HPP