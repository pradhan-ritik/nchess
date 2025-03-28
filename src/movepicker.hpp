#ifndef MOVEPICKER_HPP
#define MOVEPICKER_HPP
#include "constants.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "board.hpp"

enum MOVEGEN_STAGE {
    GENERATE_NOISY,
    GENERATE_QUIET
};

template <MOVEGEN_STAGE type> inline void generate_pawn_moves(Board& board, Movelist& movelist) {
    int pawn_direction = board.get_pawn_direction();
    BB promotion_rank = board.get_pawn_promotion_rank();
    bool turn = board.get_turn();
    BB same_team_pawns = board.get_piece_bb(PAWN, turn);

    // promotions
    BB other_team_pieces = board.get_color(!turn);
    BB empties = board.get_empties();
    BB promotion_pawns = same_team_pawns & promotion_rank;
    BB promotion_moves = 0ULL;

    while (promotion_pawns) {
        int from_ = pop_lsb(promotion_pawns);

        if constexpr (type == GENERATE_NOISY) {
            promotion_moves = pawn_attacks(bb(from_), turn) & other_team_pieces;
        }

        else if constexpr (type == GENERATE_QUIET) {
            promotion_moves = pawn_push(bb(from_), turn) & empties;
        }

        while (promotion_moves) {
            build_promotion_move(movelist, from_, pop_lsb(promotion_moves));
        } 
    }
    
    // en pessants
    if constexpr (type == GENERATE_NOISY) {
        int target = board.get_en_pessant();
        BB en_pessant_pawns = pawn_attacks(bb(target + pawn_direction), !turn) & same_team_pawns;
        // print_BB(en_pessant_pawns);
        while (en_pessant_pawns) {
            movelist.add_move(init_move(pop_lsb(en_pessant_pawns), target + pawn_direction, EN_PESSANT));
        }
    }

    // normal pawn pushes and double pawn pushes
    if constexpr (type == GENERATE_QUIET) {
        BB non_promotion_pawns = same_team_pawns & ~promotion_rank;
        // doing them all at once since single pawn pushes can not overlap
        BB single_pawn_pushes = pawn_push(non_promotion_pawns, turn) & empties;
        BB double_pawn_pushes = pawn_push(single_pawn_pushes & board.get_third_pawn_rank(), turn) & empties;

        while (single_pawn_pushes) {
            int to_ = pop_lsb(single_pawn_pushes);
            movelist.add_move(init_move(to_-pawn_direction, to_, NORMAL_MOVE));
        }
        
        while (double_pawn_pushes) {
            int to_ = pop_lsb(double_pawn_pushes);
            movelist.add_move(init_move(to_-pawn_direction-pawn_direction, to_, NORMAL_MOVE));
        }
    }
}

struct Movepicker {
    
};


#endif // MOVEPICKER_HPP