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