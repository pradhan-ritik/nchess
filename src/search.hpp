#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "constants.hpp"
#include "move.hpp"
#include "movepicker.hpp"
#include "board.hpp"
#include "evaluation.hpp"

struct Searcher {
    int best_eval;
    int starting_depth;
    Move best_move;

    int search(Board &board, int depth, int alpha, int beta);
    int start_search(Board &board, int depth);
};



#endif // SEARCH_HPP