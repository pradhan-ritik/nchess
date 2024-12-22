#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>

typedef uint16_t Move;
typedef uint32_t History;
typedef uint64_t BB; // short for Bitboard

enum FILE_RANK {
    h1, g1, f1, e1, d1, c1, b1, a1,
    h2, g2, f2, e2, d2, c2, b2, a2,
    h3, g3, f3, e3, d3, c3, b3, a3,
    h4, g4, f4, e4, d4, c4, b4, a4,
    h5, g5, f5, e5, d5, c5, b5, a5,
    h6, g6, f6, e6, d6, c6, b6, a6,
    h7, g7, f7, e7, d7, c7, b7, a7,
    h8, g8, f8, e8, d8, c8, b8, a8,
};

enum COLOR {
    WHITE,
    BLACK
};

constexpr int INF = 30'000;
constexpr int NEGINF = -INF;
// bitboards

constexpr unsigned int FIRST_SIX = 0b111111;
constexpr int MAX_LEGAL_MOVES = 218;
constexpr int MAX_CAPTURE_MOVES = 74;

constexpr BB MAX_VALUE = ~0ULL;
constexpr BB A_FILE = 0x8080808080808080ULL; // represent a file
constexpr BB B_FILE = A_FILE >> 1;
constexpr BB C_FILE = A_FILE >> 2;
constexpr BB D_FILE = A_FILE >> 3;
constexpr BB E_FILE = A_FILE >> 4;
constexpr BB F_FILE = A_FILE >> 5;
constexpr BB G_FILE = A_FILE >> 6;
constexpr BB H_FILE = A_FILE >> 7;

constexpr BB RANK_1 = 0x00000000000000FFULL; // represent first rank
constexpr BB RANK_2 = RANK_1 << 8;
constexpr BB RANK_3 = RANK_1 << 16;
constexpr BB RANK_4 = RANK_1 << 24;
constexpr BB RANK_5 = RANK_1 << 32;
constexpr BB RANK_6 = RANK_1 << 40;
constexpr BB RANK_7 = RANK_1 << 48;
constexpr BB RANK_8 = RANK_1 << 56;

constexpr BB EDGES = A_FILE | H_FILE | RANK_1 | RANK_8;

#endif // CONSTANTS_HPP