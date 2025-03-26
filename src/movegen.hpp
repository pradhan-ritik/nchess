#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include "constants.hpp"
#include "bit_operations.hpp"
#include "move.hpp"
#include "movelist.hpp"

// https://www.chessprogramming.org/Kogge-Stone_Algorithm#Occluded_Fill
inline BB _south_ray(BB pieces_to_move, BB empties) {
    pieces_to_move |= empties & (pieces_to_move >>  8);
    empties &=       (empties >>  8);
    pieces_to_move |= empties & (pieces_to_move >> 16);
    empties &=       (empties >> 16);
    pieces_to_move |= empties & (pieces_to_move >> 32);
    return pieces_to_move | (pieces_to_move >> 8);
}

inline BB _north_ray(BB pieces_to_move, BB empties) {
    pieces_to_move |= empties & (pieces_to_move <<  8);
    empties &=       (empties <<  8);
    pieces_to_move |= empties & (pieces_to_move << 16);
    empties &=       (empties << 16);
    pieces_to_move |= empties & (pieces_to_move << 32);
    return pieces_to_move | (pieces_to_move << 8);
}

inline BB _west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move << 1);
    empties &=       (empties << 1);
    pieces_to_move |= empties & (pieces_to_move << 2);
    empties &=       (empties << 2);
    pieces_to_move |= empties & (pieces_to_move << 4);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) << 1);
}

inline BB _north_west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move <<  9);
    empties &=       (empties <<  9);
    pieces_to_move |= empties & (pieces_to_move << 18);
    empties &=       (empties << 18);
    pieces_to_move |= empties & (pieces_to_move << 36);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) << 9);
}

inline BB _south_west_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~A_FILE;
    empties &= ~A_FILE;
    pieces_to_move |= empties & (pieces_to_move >>  7);
    empties &=       (empties >>  7);
    pieces_to_move |= empties & (pieces_to_move >> 14);
    empties &=       (empties >> 14);
    pieces_to_move |= empties & (pieces_to_move >> 28);
    return pieces_to_move | ((pieces_to_move & ~A_FILE) >> 7);
}

inline BB _east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move >> 1);
    empties &=       (empties >> 1);
    pieces_to_move |= empties & (pieces_to_move >> 2);
    empties &=       (empties >> 2);
    pieces_to_move |= empties & (pieces_to_move >> 4);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) >> 1);
}

inline BB _south_east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move >>  9);
    empties &=       (empties >>  9);
    pieces_to_move |= empties & (pieces_to_move >> 18);
    empties &=       (empties >> 18);
    pieces_to_move |= empties & (pieces_to_move >> 36);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) >> 9);
}

inline BB _north_east_ray(BB pieces_to_move, BB empties) {
    pieces_to_move &= ~H_FILE;
    empties &= ~H_FILE;
    pieces_to_move |= empties & (pieces_to_move <<  7);
    empties &=       (empties <<  7);
    pieces_to_move |= empties & (pieces_to_move << 14);
    empties &=       (empties << 14);
    pieces_to_move |= empties & (pieces_to_move << 28);
    return pieces_to_move | ((pieces_to_move & ~H_FILE) << 7);
}

inline BB pawn_attacks_white(BB white_pawns) {
    return (
        ((white_pawns & ~(A_FILE)) << 9) |
        ((white_pawns & ~(H_FILE)) << 7)
    );
}

inline BB pawn_attacks_black(BB black_pawns) {
    return (
        ((black_pawns & ~(H_FILE)) >> 9) |
        ((black_pawns & ~(A_FILE)) >> 7)
    );
}

inline BB pawn_attacks(BB pawns, bool turn) {
    return turn ? pawn_attacks_black(pawns) : pawn_attacks_white(pawns);
}

inline BB pawn_push(BB pawns, bool turn) {
    return turn ? (pawns >> 8) : (pawns << 8);
}

inline BB knight_attacks(BB knights) {
    return (
        ((knights & ~(A_FILE | B_FILE)) >> 6) |
        ((knights & ~(G_FILE | H_FILE)) >> 10) |
        ((knights & ~(G_FILE | H_FILE)) << 6) |
        ((knights & ~(A_FILE | B_FILE)) << 10) | 
        ((knights & ~(         A_FILE)) >> 15) | 
        ((knights & ~(         H_FILE)) >> 17) |
        ((knights & ~(         H_FILE)) << 15) |
        ((knights & ~(         A_FILE)) << 17)
        );
}

inline BB bishop_attacks(BB bishops, BB empties) {
    return (
        _north_east_ray(bishops, empties) |
        _north_west_ray(bishops, empties) |
        _south_east_ray(bishops, empties) |
        _south_west_ray(bishops, empties)
    );
}

inline BB rook_attacks(BB rooks, BB empties) {
    return (
        _north_ray(rooks, empties) |
        _south_ray(rooks, empties) |
        _east_ray(rooks, empties) |
        _west_ray(rooks, empties)
    );
}

inline BB queen_attacks(BB queens, BB empties) {
    return (
        _north_ray(queens, empties) |
        _south_ray(queens, empties) |
        _east_ray(queens, empties) |
        _west_ray(queens, empties) |
        _north_east_ray(queens, empties) |
        _north_west_ray(queens, empties) |
        _south_east_ray(queens, empties) |
        _south_west_ray(queens, empties)
    );
} 

inline BB king_attacks(BB king) {
    return (
        (king & ~(A_FILE)) << 9 |
        (king & ~(A_FILE)) << 1 |
        (king & ~(A_FILE)) >> 7 |
        (king & ~(H_FILE)) << 7 |
        (king & ~(H_FILE)) >> 1 |
        (king & ~(H_FILE)) >> 9 |
        (king            ) << 8 |
        (king            ) >> 8  
    );
}

inline BB psuedo_legal_moves_of(BB moves, BB same_team) {
    return moves & ~(same_team);
}

inline void build_normal_moves(Movelist& movelist, BB moves, int from_) {
    while (moves) {
        movelist.add_move(init_move(from_, pop_lsb(moves), NORMAL_MOVE));
    }
}

inline void build_promotion_move(Movelist &movelist, int from_, int to_) {
    movelist.add_move(init_move(from_, to_, PROMOTION, QUEEN));
    movelist.add_move(init_move(from_, to_, PROMOTION, ROOK));
    movelist.add_move(init_move(from_, to_, PROMOTION, BISHOP));
    movelist.add_move(init_move(from_, to_, PROMOTION, KNIGHT));
}

#endif // MOVEGEN_HPP