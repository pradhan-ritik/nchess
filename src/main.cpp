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

    return 0;
}
