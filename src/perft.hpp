#ifndef PERFT_HPP
#define PERFT_HPP

#include "constants.hpp"
#include "board.hpp"
#include "movelist.hpp"
#include "movepicker.hpp"

BB perft(Board& board, int depth);
BB perft_info(Board& board, int depth);



#endif // PERFT_HPP