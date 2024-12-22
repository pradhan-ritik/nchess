#ifndef HISTORY_HPP
#define HISTORY_HPP
#include "constants.hpp"
#include "bit_operations.hpp"
#include "piece.hpp"
#include "move.hpp"

#define EMPTY_HISTORY init_history(0, EMPTY, 0)

/*
History object
first 16 bits: move
next 3 bits: captured piece
next 10 bits rules
29 bits total
rounded to 32
*/

inline History init_history(Move move, PIECE captured_piece, uint16_t rules) {
    return move | (captured_piece << 16) | (rules << 19);
}

inline Move move_of(History history) {
    return history & 0xFFFF;
}

inline PIECE captured_piece_of(History history) {
    return PIECE((history >> 16) & 0b111);
}

inline uint16_t rules_of(History history) {
    return history >> 19;
}

inline int en_pessant_of(History history) {
    return (history >> 19) & 0b111111;
}

inline CASTLE_SIDE get_castle(History history, CASTLE_SIDE side, bool turn_) {
    return (history >> (25+2*turn_)) & side;
} 


#endif // HISTORY_HPP