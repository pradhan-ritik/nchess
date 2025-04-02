#include "search.hpp"

int Searcher::search(Board &board, int depth, int alpha, int beta) {
    alpha = std::max(alpha, NEGINF + (starting_depth - depth));
    beta = std::min(beta, INF - (starting_depth - depth));

    if (alpha >= beta) { // mate has been found earlier in the search
        return alpha;
    }

    if (current_time() > search_limit) {
        return 0;
    }

    if (depth == 0) {
        return evaluate(board) * -((board.get_turn())*2 - 1);
    }

    int best_eval_this_depth = NEGINF;
    Movepicker mp = Movepicker(&board);
    Move move;
    bool generated_moves = false;
    while ((move = mp.next_move()) != NULL_MOVE) {
        board.play_move(move);
        if (!board.was_move_legal()) {
            board.undo_last_move();
            continue;
        }
    
        generated_moves = true;
        int evaluation = -search(board, depth - 1, -beta, -alpha);
        board.undo_last_move();
        best_eval_this_depth = std::max(evaluation, best_eval_this_depth);
        
        if (evaluation > alpha) {
            alpha = evaluation;
            if (depth == starting_depth) { // original depth
                best_move = move;
                best_eval = evaluation;
            }
        }
        
        if (evaluation >= beta) {
            return beta;
        }

        if (current_time() > search_limit) {
            return 0;
        }
    }

    if (!generated_moves) {
        if (board.king_attackers()) { // check and there are no moves | Checkmate
            return NEGINF + (starting_depth - depth);
        }
        
        return 0; // else it has to be a stalemate 
    }

    return best_eval_this_depth;
}

int Searcher::start_search(Board &board, int max_time, int max_depth) {
    search_limit = current_time() + max_time; 

    // we do this so even if there is no time, there will stil be something to play
    search(board, 1, NEGINF, INF);

    for (int i = 2; i <= max_depth; i++) {
        starting_depth = i;
        search(board, i, NEGINF, INF);

        if (current_time() > search_limit) {
            break;
        }
    }

    return best_eval;
} 

