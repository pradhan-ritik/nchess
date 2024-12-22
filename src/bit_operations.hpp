#ifndef BIT_OPERATIONS_HPP
#define BIT_OPERATIONS_HPP

#include "constants.hpp"

inline void print_BB(BB bitboard, int length=64) {
    int cur;
    for (int i = length-1; i >= 0; i--) {
        cur = bitboard >> i;

        if ((i+1) % 8 == 0) {
            printf("\n");
        }

        if (cur & 1) {
            printf("1 ");
        }

        else {
            printf("0 ");
        }
    }
    printf("\n");
}

inline BB bb(int pos) {
    return 1ULL << pos;
}

inline int count_bits(BB bitboard) {
    return __builtin_popcountll(bitboard);
}

inline int lsb(BB bitboard) {
    return __builtin_ctzll(bitboard);
}

inline int pop_lsb(BB& bitboard) {
    int first = lsb(bitboard);
    bitboard &= bitboard - 1;
    return first;
}

inline void set_bit_on(BB& bitboard, int index) {
    bitboard |= bb(index);
}

inline void set_bit_off(BB& bitboard, int index) {
    bitboard &= ~bb(index);
}

inline void toggle_bit(BB& bitboard, int index) {
    bitboard ^= bb(index);
}

inline BB get_bit(BB bitboard, int index) {
    return bitboard & bb(index);
}

inline bool is_within_board(int pos) {
    return pos >= 0 && pos < 64;
}

inline int east(int pos) {
    return pos - 1;
}

inline int west(int pos) {
    return pos + 1;
}

inline int north(int pos) {
    return pos + 8;
}

inline int south(int pos) {
    return pos - 8;
}

inline int north_east(int pos) {
    return north(east(pos));
}

inline int north_west(int pos) {
    return north(west(pos));
}

inline int south_west(int pos) {
    return south(west(pos));
}

inline int south_east(int pos) {
    return south(east(pos));
}
// pos = 0b11111 = 63
// first three bits tell use file, last three tell us rank

inline int file(const int pos) {
    return pos & 7; // last three bits
}

inline int rank(const int pos) {
    return pos >> 3; // first three bits
}

#endif // BIT_OPERATIONS_HPP