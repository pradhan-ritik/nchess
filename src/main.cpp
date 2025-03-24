#include "constants.hpp"
#include "piece.hpp"
#include "bit_operations.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "board.hpp"
#include "movepicker.hpp"

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
    const char* base_fen = "rq2k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K1QR w KQkq - 0 1";
    Board b = Board(base_fen);
    b.display_game();
    Movelist movelist;

    // generate_pawn_moves<GENERATE_NOISY>(b, movelist);
    // generate_pawn_moves<GENERATE_QUIET>(b, movelist);
    // generate_major_piece_moves<GENERATE_NOISY, KNIGHT>(b, movelist);
    // generate_major_piece_moves<GENERATE_QUIET, KNIGHT>(b, movelist);
    // generate_major_piece_moves<GENERATE_NOISY, BISHOP>(b, movelist);
    // generate_major_piece_moves<GENERATE_QUIET, BISHOP>(b, movelist);
    // generate_major_piece_moves<GENERATE_NOISY, ROOK>(b, movelist);
    // generate_major_piece_moves<GENERATE_QUIET, ROOK>(b, movelist);
    // generate_major_piece_moves<GENERATE_NOISY, QUEEN>(b, movelist);
    // generate_major_piece_moves<GENERATE_QUIET, QUEEN>(b, movelist);
    generate_king_moves<GENERATE_NOISY>(b, movelist);
    generate_king_moves<GENERATE_QUIET>(b, movelist);

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
