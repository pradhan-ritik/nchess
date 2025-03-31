#include "constants.hpp"
#include "piece.hpp"
#include "bit_operations.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "board.hpp"
#include "movepicker.hpp"
#include "perft.hpp"
#include "interface.hpp"
#include "evaluation.hpp"
#include "search.hpp"

/*
pawn move generation fen
WHITE:
n1n4k/1P3p2/1P6/1Pp3p1/5pP1/5P2/P1p5/1N1N3K w - c5 0 1
BLACK:
n1n4k/1P3p2/1P6/1Pp3p1/5pP1/5P2/P1p5/1N1N3K b - g4 0 1
Major move generation fen
WHITE:
K7/8/1qRnb3/8/8/1QrNB3/8/k7 w - - 0 1
BLACK:
K7/8/1qRnb3/8/8/1QrNB3/8/k7 b - - 0 1
castle fens:
WHITE:
r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1
BLACK:
r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1

8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3
*/

int main() {
    // INTERFACE MODE
    const char* base_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Board b = Board(base_fen);
    Searcher s; 
    Interface i = Interface(&b, &s);
    i.run();
    // INTERFACE MODE

    // const char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // Board b = Board(fen);
    // Searcher s;
    // Searcher *s2 = &s;
    // int evaluation = s2->start_search(b, 3);
    // printf("bestmove %s eval %i\n", move_to_uci(s2->best_move), evaluation);
    // printf("EVALU;ATION: %i\n", evaluate(b));
    // b.set_game(fen);
    // b.play_move(init_move(d2, d3, NORMAL_MOVE));
    // b.undo_last_move();
    // b.play_move(init_move(a2, a3, NORMAL_MOVE));
    // b.display_game();
    // print_BB(b.get_color());
    // print_BB(b.get_color(b.other_turn()));

    // print_BB(b.get_color(b.other_turn()));
    // printf("---------------------");
    // b.play_move(init_move(g7, b2, NORMAL_MOVE));
    // print_BB(b.get_color(b.get_turn()));
    // print_BB(b.get_color(b.other_turn()));
    // printf("---------------------");
    // b.undo_last_move();
    // print_BB(b.get_color(b.get_turn()));
    // print_BB(b.get_color(b.other_turn()));
    // b.display_game();
    // b.display_game();
    // print_BB(b.was_move_legal());
    // perft_info(b, 6);

    return 0;
}
