#include "perft.hpp"

BB perft(Board& board, int depth) {
    if (depth == 0) {
        return 1ULL;
    }

    BB nodes = 0ULL;
    Movepicker mp = Movepicker(&board);
    
    Move move;
    while ((move = mp.next_move()) != NULL_MOVE) {
        // printf("here %s\n", move_to_uci(move));
        if (depth != 1) {
            board.play_move(move);
            if (board.was_move_legal()) {
                nodes += perft(board, depth - 1);
            }
            board.undo_last_move();
        }
        else nodes++;
    }

    return nodes;
}

BB perft_divide(Board& board, int depth) {
    if (depth == 0) {
        return 1ULL;
    }

    BB nodes_here = 0ULL;
    BB nodes = 0ULL;
    Movepicker mp = Movepicker(&board);
    
    Move move;
    while ((move = mp.next_move()) != NULL_MOVE) {
        if (depth != 1) {
            board.play_move(move);
            if (board.was_move_legal()) {
                nodes_here = perft(board, depth - 1);
            }

            nodes += nodes_here;

            printf("%s: %llu\n", move_to_uci(move), nodes_here);

            board.undo_last_move();
        }
        else nodes++;
    }

    return nodes;
}

BB perft_info(Board& board, int depth) {
    printf("Nodes searched: %llu\n", perft_divide(board, depth));
}