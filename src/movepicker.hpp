#ifndef MOVEPICKER_HPP
#define MOVEPICKER_HPP
#include "constants.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "board.hpp"

enum MOVEGEN_STAGE {
    GENERATE_PV,
    GENERATE_NOISY,
    GENERATE_QUIET,
    MOVEGEN_FINISHED
};

template <MOVEGEN_STAGE stage> inline int generate_pawn_moves(Board& board, Movelist& movelist) {
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

        if constexpr (stage == GENERATE_NOISY) {
            promotion_moves = pawn_attacks(bb(from_), turn) & other_team_pieces;
        }

        else if constexpr (stage == GENERATE_QUIET) {
            promotion_moves = pawn_push(bb(from_), turn) & empties;
        }

        while (promotion_moves) {
            build_promotion_move(movelist, from_, pop_lsb(promotion_moves));
        } 
    }

    // pawn normal captures
    BB non_promotion_pawns = same_team_pawns & ~promotion_rank;
    if constexpr (stage == GENERATE_NOISY) {
        // Realized that there is no need to make a copy because the stage can never be both at the same time 
        // BB copy_non_promotion_pawns = non_promotion_pawns;
        while (non_promotion_pawns) {
            int from_ = pop_lsb(non_promotion_pawns);
            BB attacks = pawn_attacks(bb(from_), turn) & other_team_pieces;
            while (attacks) {
                int to_ = pop_lsb(attacks);
                movelist.add_move(init_move(from_, to_, NORMAL_MOVE));
            }
        }
    }

    // en pessants
    if constexpr (stage == GENERATE_NOISY) {
        int target = board.get_en_pessant();
        // printf("TARGET %i\n", target);
        BB en_pessant_pawns = pawn_attacks(bb(target), !turn) & same_team_pawns;
        // print_BB(en_pessant_pawns);
        while (en_pessant_pawns) {
            movelist.add_move(init_move(pop_lsb(en_pessant_pawns), target, EN_PESSANT));
        }
    }

    // normal pawn pushes and double pawn pushes
    if constexpr (stage == GENERATE_QUIET) {
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

    return movelist.get_length();
}

template <MOVEGEN_STAGE stage, PIECE piece> inline int generate_major_piece_moves(Board& board, Movelist& movelist) {
    static_assert(piece != PAWN && piece != KING);
    bool turn = board.get_turn();
    BB same_team_pieces = board.get_piece_bb(piece, turn);
    BB no_hit = board.get_color(); // same team
    BB empties = board.get_empties();

    if constexpr (stage == GENERATE_NOISY) {
        no_hit |= empties;
    }

    else if constexpr (stage == GENERATE_QUIET) {
        no_hit |= board.get_color(!turn); // other team
    }
    
    BB moves = 0ULL;
    while (same_team_pieces) {
        int from_ = pop_lsb(same_team_pieces);
        if constexpr (piece == KNIGHT)
            moves = knight_attacks(bb(from_));

        else if constexpr (piece == BISHOP)
            moves = bishop_attacks(bb(from_), empties);

        else if constexpr (piece == ROOK)
            moves = rook_attacks(bb(from_), empties);

        else if constexpr (piece == QUEEN)
            moves = queen_attacks(bb(from_), empties);

        moves &= ~no_hit;

        while (moves) {
            movelist.add_move(init_move(from_, pop_lsb(moves), NORMAL_MOVE));
        }
    }

    return movelist.get_length();
}

template <MOVEGEN_STAGE stage> inline int generate_king_moves(Board& board, Movelist& movelist) {
    bool turn = board.get_turn();
    BB same_team_king = board.get_piece_bb(KING, turn);
    BB no_hit = board.get_color() | king_attacks(board.get_piece_bb(KING, !turn)); // same team
    BB empties = board.get_empties();

    if (stage == GENERATE_NOISY) {
        no_hit |= empties;
    }

    if (stage == GENERATE_QUIET) {
        no_hit |= board.get_color(!turn); // other team
    }

    BB moves = king_attacks(same_team_king) & ~no_hit;
    int from_ = pop_lsb(same_team_king);
    while (moves) {
        movelist.add_move(init_move(from_, pop_lsb(moves), NORMAL_MOVE));
    }

    if (stage == GENERATE_QUIET && !board.king_attackers()) {
        int shift = turn * 56;
        if (board.get_castle(KINGSIDE, turn) && in_BB(empties, (0b110ULL << shift))) {
            BB attack_mask = 0b110ULL << shift;
            bool squares_under_attack = false;
            while (attack_mask) {
                int pos = pop_lsb(attack_mask);
                squares_under_attack |= bool(board.square_attackers(pos));
            }

            if (!squares_under_attack)
                movelist.add_move(init_move(from_, shift + 1, CASTLE));
        }

        // queenside castle
        if (board.get_castle(QUEENSIDE, turn) && in_BB(empties, (0b1110000ULL << shift))) {
            BB attack_mask = 0b110000ULL << shift;
            bool squares_under_attack = false;
            while (attack_mask) {
                int pos = pop_lsb(attack_mask);
                squares_under_attack |= bool(board.square_attackers(pos));
            }

            if (!squares_under_attack)
                movelist.add_move(init_move(from_, shift + 5, CASTLE));
        }
    }

    return movelist.get_length();
}

template <MOVEGEN_STAGE stage> inline int generate_moves(Board& board, Movelist& movelist) {
    generate_pawn_moves<stage>(board, movelist);
    generate_major_piece_moves<stage, KNIGHT>(board, movelist);
    generate_major_piece_moves<stage, BISHOP>(board, movelist);
    generate_major_piece_moves<stage, ROOK>(board, movelist);
    generate_major_piece_moves<stage, QUEEN>(board, movelist);
    generate_king_moves<stage>(board, movelist);
    return movelist.get_length();
}

struct Movepicker {
    private:
    Board* board;
    Movelist movelist;
    Move pv_move;
    int movelist_length;
    int stage;

    public:
    Movepicker(Board* board, Move pv_move=NULL_MOVE);
    Move next_move();
};


#endif // MOVEPICKER_HPP
