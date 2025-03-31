#include "search.hpp"

int Searcher::search(Board &board, int depth, int alpha, int beta) {
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
        // printf("HERE depth %i move %s\n", depth, move_to_uci(move));
        // board.next_turn();
        // print_BB(board.king_attackers());
        // board.display_game(true);
        // board.next_turn();
    
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
    }

    if (!generated_moves) {
        if (board.king_attackers()) { // check and there are no moves | Checkmate
            return NEGINF + (starting_depth - depth);
        }
        
        return 0; // else it has to be a stalemate 
    }

    return best_eval_this_depth;
}

int Searcher::start_search(Board &board, int depth) {
    starting_depth = depth;
    int evaluation = search(board, depth, NEGINF, INF);
    return evaluation;
} 