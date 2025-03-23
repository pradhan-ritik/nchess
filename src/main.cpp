#include "constants.hpp"
#include "piece.hpp"
#include "bit_operations.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "board.hpp"
#include "movepicker.hpp"

/*
PAWN MOVE GENERATION FENS
JUST TEST ALL ARND FEN
test fens
white pawn single and double pawn push
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0

black pawn single and double pawn push
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 0

white en pessant
rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3

black en pessant
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 0

white promotion
rnbqkbnr/ppP1p1pp/8/8/8/8/PPPP1P1p/RNBQKBNR w KQkq - 0 6

black promotion
rnbqkbnr/ppP1p1pp/8/8/8/8/PPPP1P1p/RNBQKBNR n KQkq - 0 6


all around fen
WHITE:
n1n4k/1P3p2/1P6/1Pp3p1/5pP1/5P2/P1p5/1N1N3K w - c5 0 1
BLACK:
n1n4k/1P3p2/1P6/1Pp3p1/5pP1/5P2/P1p5/1N1N3K b - g4 0 1

*/

int main() {
    const char* base_fen = "n1n4k/1P3p2/1P6/1Pp3p1/5pP1/5P2/P1p5/1N1N3K w - c5 0 1";
    Board b = Board(base_fen);
    b.display_game();
    Movelist movelist;

    generate_pawn_moves<GENERATE_NOISY>(b, movelist);
    generate_pawn_moves<GENERATE_QUIET>(b, movelist);
    generate_knight_moves<GENERATE_NOISY>(b, movelist);
    generate_knight_moves<GENERATE_QUIET>(b, movelist);

    movelist.print_moves();

    // printf("%s", move_to_uci(init_move(h7, h8, PROMOTION, QUEEN)));
    // 0.4 with optimizations
    // for (int i = 0; i < 10000000; i++)
    //     is_move_legal_copying(b, init_move(e2, c4, NORMAL_MOVE));

    // 0.17 with optimizations
    // for (int i = 0; i < 100000000; i++)
    //     b.is_move_legal(init_move(e2, c4, NORMAL_MOVE));

    return 0;
}
