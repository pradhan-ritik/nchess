#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include <iostream>
#include <sstream>
#include <numeric>
#include "constants.hpp"
#include "board.hpp"
#include "perft.hpp"
#include "evaluation.hpp"
#include "search.hpp"

std::vector<std::string> tokenize(std::string& s, char del);
struct Interface {
    private:
    Board *board;
    Searcher *searcher;
    bool running;
    
    public:
    Interface(Board* board, Searcher *searcher);
    void run();
    void run_command(std::string& args);
    void position(std::vector<std::string>& args_vector, int length);
    void go(std::vector<std::string>& args_vector, int length);
};

inline Move uci_to_move(std::string uci, Board& board) {
    MOVE_TYPE move_type = NORMAL_MOVE;
    PIECE promotion_piece = EMPTY;
    int pos_1 = ((uci[0]-'a')^7) + 8*(uci[1]-'1');
    int pos_2 = ((uci[2]-'a')^7) + 8*(uci[3]-'1');
    int abs_distance = abs(pos_1 - pos_2);
    // if the king moves 2 to the side, it means it is a castle
    if (board.get_piece_on_pos(pos_1) == KING && abs_distance == 2) {
        move_type = CASTLE;
    }

    if ((abs_distance == 7 || abs_distance == 9) && board.get_piece_on_pos(pos_1) == PAWN && board.get_piece_on_pos(pos_2) == EMPTY) {
        move_type = EN_PESSANT;
    }

    if (uci.length() > 4) {
        move_type = PROMOTION;
        if (uci[4] == 'q') 
            promotion_piece = QUEEN;
        else if (uci[4] == 'r')
            promotion_piece = ROOK;
        else if (uci[4] == 'b')
            promotion_piece = BISHOP;
        else if (uci[4] == 'k')
            promotion_piece = KNIGHT;
    }

    return init_move(
        pos_1,
        pos_2,
        move_type,
        promotion_piece
    );
}

#endif // INTERFACE_HPP
