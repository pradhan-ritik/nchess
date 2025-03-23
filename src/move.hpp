#ifndef MOVE_HPP
#define MOVE_HPP

enum MOVE_TYPE {
    NORMAL_MOVE = 0b0000,
    PROMOTION = 0b0001,
    CASTLE = 0b0010,
    EN_PESSANT = 0b0100,
};

#define CASTLE_SIDE uint8_t
#define KINGSIDE 0b01u
#define QUEENSIDE 0b10u

#include "constants.hpp"
#include "piece.hpp"

inline Move init_move(int from, int to, MOVE_TYPE move_type, PIECE promotion_piece=EMPTY) {
    return Move(from | (to << 6) | (move_type << 12) | (promotion_piece << 13));
}

inline int from(Move move) {
    return move & 0b111111;
}

inline int to(Move move) {
    return (move >> 6) & 0b111111;
}

inline MOVE_TYPE move_type(Move move) {
    return MOVE_TYPE(move >> 12);
}

inline bool is_normal_move(Move move) {
    return !(move >> 12);
}

inline bool is_promotion(Move move) {
    return (move >> 12) & PROMOTION;
} 

inline PIECE promotion_piece(Move move) {
    return PIECE((move >> 13));
}

inline bool is_castle(Move move) {
    return (move >> 12) == CASTLE;
}

inline CASTLE_SIDE castle_side(Move move) {
    return CASTLE_SIDE((from(move) - to(move)) < 0) + 1;
}

inline bool is_en_pessant(Move move) {
    return (move >> 12) == EN_PESSANT;
}

inline const char* move_to_uci(Move move) {
    std::string uci_move = "";
    uci_move += ((from(move) & 7)^7) + 'a';
    uci_move += (from(move) / 8) + '1';
    uci_move += ((to(move) & 7)^7) + 'a';
    uci_move += (to(move) / 8) + '1';

    if (is_en_pessant(move)) {
        return uci_move.c_str();
    }

    if (promotion_piece(move) == QUEEN) {
        uci_move += 'q';
    }

    if (promotion_piece(move) == ROOK) {
        uci_move += 'r';
    }

    if (promotion_piece(move) == BISHOP) {
        uci_move += 'b';
    }

    if (promotion_piece(move) == KNIGHT) {
        uci_move += 'n';
    }

    return uci_move.c_str();
}

#endif // MOVE_HPP