#include "constants.hpp"
#include "piece.hpp"
#include "bit_operations.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "board.hpp"

int main() {
    const char* base_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
    Board b = Board(base_fen);
    // 0.4 with optimizations
    // for (int i = 0; i < 10000000; i++)
    //     is_move_legal_copying(b, init_move(e2, c4, NORMAL_MOVE));

    // 0.17 with optimizations
    for (int i = 0; i < 100000000; i++)
        b.is_move_legal(init_move(e2, c4, NORMAL_MOVE));

    return 0;
}
