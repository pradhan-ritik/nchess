#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <chrono>
#include "constants.hpp"
#include "move.hpp"
#include "movepicker.hpp"
#include "board.hpp"
#include "evaluation.hpp"

typedef std::chrono::milliseconds::rep Time;
inline Time current_time() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

struct Searcher {
    int best_eval;
    int starting_depth;
    Move best_move;
    Move pv_move;
    Time search_limit;

    inline int get_time_for_move(int total_time) {
        return total_time / 10;
    }
    int search(Board &board, int depth, int alpha, int beta);
    int start_search(Board &board, int max_time, int max_depth=255);
};



#endif // SEARCH_HPP
