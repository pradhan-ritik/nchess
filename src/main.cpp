#include "constants.hpp"
#include "piece.hpp"
#include "bit_operations.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "board.hpp"
#include "movepicker.hpp"
#include "perft.hpp"
#include "interface.hpp"
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
*/

int main() {
    // INTERFACE MODE
    const char* base_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Board b = Board(base_fen);
    Interface i = Interface(&b);
    i.run();
    // INTERFACE MODE

    // const char* fen = "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2";
    // Board b = Board(fen);
    // print_BB(b.get_color(b.get_turn()));
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
    // perft_info(b, 3);

    return 0;
}
