#include "search.hpp"

int Searcher::search(Board &board, int depth, int alpha, int beta) {
    if (current_time() > search_limit) {
        return 0;
    } 

    alpha = std::max(alpha, NEGINF + (starting_depth - depth));
    beta = std::min(beta, INF - (starting_depth - depth));

    if (alpha >= beta) { // mate has been found earlier in the search
        return alpha;
    } 

    if (depth == 0) {
        return evaluate(board) * -((board.get_turn())*2 - 1);
    }

    int best_eval_this_depth = NEGINF;
    Movepicker mp = Movepicker(&board, pv_move);
    Move move;
    bool generated_moves = false;
    while ((move = mp.next_move()) != NULL_MOVE) {
        board.play_move(move);
        if (!board.was_move_legal()) {
            board.undo_last_move();
            continue;
        }
    
        generated_moves = true;
        if (depth == 5)
            last_move = move;
        int evaluation = -search(board, depth - 1, -beta, -alpha);
        // if (!(starting_depth == 6 && depth == 4 && last_move == init_move(g2, g3, NORMAL_MOVE))) {
            board.undo_last_move();
        // }
        // else {
        //     printf("Move: %s last_move: %s\n", move_to_uci(move).c_str(), move_to_uci(last_move).c_str());
        //         board.display_game();
        //         board.debug = true;
        //         board.undo_last_move();
        //         board.debug = false;
        // }
        // if (starting_depth > 3 && starting_depth < 6 && depth == starting_depth)
        //     printf("PIECE %i on f4 depth %i starting depth %i currmove %s\n", board.get_piece_on_pos(f4), depth, starting_depth, move_to_uci(move).c_str());
        // if (board.get_piece_on_pos(f4) != BISHOP && depth == starting_depth) {
        //     printf("On depth %i, starting_depth %i Bishop has been turned into rook, Move %s\n", depth, starting_depth, move_to_uci(move).c_str());
        // }
        // if (starting_depth == 6 && depth == 4) {
        //     if (last_move == init_move(d1, c1, NORMAL_MOVE)) {
        //         printf("Move: %s last_move: %s\n", move_to_uci(move).c_str(), move_to_uci(last_move).c_str());
        //         board.debug = true;
        //         board.undo_last_move();
        //         board.debug = false;
        //         board.display_game();
        //     }
        //     // printf("PIECE %i on f4 depth\n", board.get_piece_on_pos(f4));
        // }

        if (current_time() > search_limit) {
            return 0;
        }

        best_eval_this_depth = std::max(evaluation, best_eval_this_depth); 

        if (evaluation > alpha) {
            alpha = evaluation;
            if (depth == starting_depth) { // original depth
                // printf("At depth %i, New best move: %s, is psuedo legal? %i\n", depth, move_to_uci(move).c_str(), board.is_move_pseudo_legal(move));
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

int Searcher::start_search(Board &board, int max_time, int max_depth) {
    search_limit = current_time() + max_time; 

    // Even if there is no time, there will stil be something to play
    starting_depth = 1;
    pv_move = NULL_MOVE;
    search(board, 1, NEGINF, INF);
    pv_move = best_move;
    printf("info depth 1 time 0 pv %s score %i\n", move_to_uci(best_move).c_str(), best_eval);

    for (int i = 2; i <= max_depth; i++) {
        starting_depth = i;
        search(board, i, NEGINF, INF);
        pv_move = best_move;
        printf("info depth %i time %li pv %s score %i\n", i, current_time() - search_limit + max_time, move_to_uci(best_move).c_str(), best_eval);

        if (current_time() > search_limit) {
            break;
        }
    }

    return best_eval;
}
