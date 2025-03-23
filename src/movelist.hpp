#ifndef MOVELIST_HPP
#define MOVELIST_HPP
#include "constants.hpp"
#include "move.hpp"

struct Movelist {
    private:
    Move moves[MAX_LEGAL_MOVES];
    int16_t index;
    public:
    Movelist() : index(-1) {}

    inline void add_move(Move move) {
        moves[++index] = move;
    }

    inline void remove_move(Move move) {
        index--;
    }

    inline Move get_move_at(int index_) {
        return moves[index_];
    }

    inline Move get_last_move() {
        return moves[index];
    }
 
    inline int get_length() {
        return index+1;
    }

    inline void print_moves() {
        for (int i = 0; i <= index; i++) {
            printf("%s\n", move_to_uci(moves[i]));
        }
    }

};


#endif // MOVELIST_HPP