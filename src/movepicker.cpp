#include "movepicker.hpp"

Movepicker::Movepicker(Board* board) {
    this->board = board;
    this->movelist = Movelist();
    this->movelist_length = 0;
    this->stage = 0;
}

Move Movepicker::next_move() {
    switch (stage) {
        case GENERATE_NOISY:
            if (movelist_length == 0) { // need to generate moves
                movelist_length = generate_moves<GENERATE_NOISY>(*board, movelist);
                movelist.reset_index();
            }

            if (movelist.plus_plus_index() == movelist_length) {
                stage++;
                movelist_length = 0;
                movelist.reset_index();
            }

            else {
                return movelist.get_move_at(movelist.get_index());
            }

        case GENERATE_QUIET:
        if (movelist_length == 0) { // need to generate moves
            movelist_length = generate_moves<GENERATE_QUIET>(*board, movelist);
            movelist.reset_index();
        }

        if (movelist.plus_plus_index() == movelist_length) {
            stage++;
            movelist_length = 0;
        }

        else {
            return movelist.get_move_at(movelist.get_index());
        }

        case MOVEGEN_FINISHED:
            return NULL_MOVE;
    }
}