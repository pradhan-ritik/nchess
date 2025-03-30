#include "evaluation.hpp"

int evaluate(Board &board) {
    int evaluation = 0;
    BB pieces = ~board.get_empties();
    BB white = board.get_color(WHITE);
    int cur_eval = 0;
    while (pieces) {
        int pos = pop_lsb(pieces);
        PIECE piece = board.get_piece_at_pos(pos);
        switch (piece) {
            case PAWN:
                cur_eval += PAWN_VALUE;
        }

    }
}
