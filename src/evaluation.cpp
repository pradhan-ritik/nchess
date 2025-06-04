#include "evaluation.hpp"

int evaluate(Board &board) {
    int evaluation = 0;
    BB black = board.get_color(BLACK);
    BB pieces = board.get_color(WHITE) | black;
    int cur_eval = 0;
    while (pieces) {
        int pos = pop_lsb(pieces);
        bool is_black = bb(pos) & black;
        PIECE piece = board.get_piece_on_pos(pos);
        pos ^= 56 * is_black;
        switch (piece) {
            case PAWN:
                cur_eval = PAWN_VALUE + PAWN_PST[pos];
                break;

            case KNIGHT:
                cur_eval = KNIGHT_VALUE + KNIGHT_PST[pos];
                break;
            
            case BISHOP:
                cur_eval = BISHOP_VALUE + BISHOP_PST[pos];
                break;

            case ROOK:
                cur_eval = ROOK_VALUE + ROOK_PST[pos];
                break;

            case QUEEN:
                cur_eval = QUEEN_VALUE + QUEEN_PST[pos];
                break;

            case KING:
                cur_eval = KING_VALUE + KING_PST[pos];
                break;
        }

        if (is_black) 
            cur_eval *= -1;

        evaluation += cur_eval;
        cur_eval = 0;
        // printf("%i ", evaluation);
    }
    return evaluation;
}
