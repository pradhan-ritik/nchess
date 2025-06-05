#include "movepicker.hpp"

Movepicker::Movepicker(Board* board, Move pv_move) {
    this->board = board;
    this->movelist = Movelist();
    this->pv_move = pv_move;
    this->movelist_length = 0;
    this->stage = GENERATE_PV;
    if (pv_move == NULL_MOVE || !board->is_move_pseudo_legal(pv_move)) // skip GENERATE_PV if no pv_move or pv_move is illegal
        stage++; 
}

Move Movepicker::next_move() {
    while (true) {
    switch (stage) {
        case GENERATE_PV:
            stage++;
            return pv_move;

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
                Move move = movelist.get_move_at(movelist.get_index());
                if (move != pv_move)
                    return move;
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
                Move move = movelist.get_move_at(movelist.get_index());
                if (move != pv_move)
                    return move;
            }

        case MOVEGEN_FINISHED:
            return NULL_MOVE;
    }
    }
    
    return NULL_MOVE;
}
