#include "interface.hpp"

std::vector<std::string> tokenize(std::string& s, char del) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline(ss, item, del)) {
        result.push_back(item);
    }

    return result;
}

Interface::Interface(Board* board, Searcher* searcher) {
    this->board = board;
    this->searcher = searcher;
    this->running = false;
}

void Interface::run() {
    running = true;
    std::string args;
    while (running && getline(std::cin, args)) {
        run_command(args);
    }
}

void Interface::run_command(std::string& args) {
    
    std::vector<std::string> args_vector = tokenize(args, ' ');
    int length = args_vector.size();

    if (args_vector[0] == "quit") {
        running = false;
    }

    else if (args_vector[0] == "position") {
        position(args_vector, length);
    }

    else if (args_vector[0] == "go") {
        go(args_vector, length);
    }

    else if (args_vector[0] == "uci") {
        printf("uciok\n");
    }

    else if (args_vector[0] == "ucinewgame") {
        board->set_game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", false);
    }

    else if (args_vector[0] == "isready") {
        printf("readyok\n");
    }

    else if (args_vector[0] == "eval") {
        printf("Eval: %i\n", evaluate(*board));
    }

}

void Interface::position(std::vector<std::string>& args_vector, int length) {
    int moves_pos = 0;
    if (args_vector[1] == "fen") {
        moves_pos = 9;
        // printf("fen: %s", std::accumulate(args_vector.begin()+2, args_vector.end(), std::string(""), [](std::string& a, std::string &b) {return a + " " + b;}).c_str());
        board->set_game(std::accumulate(args_vector.begin()+2, args_vector.end(), std::string(""), [](std::string& a, std::string &b) {return a + " " + b;}).c_str());
        // board->display_game();
    }

    if (moves_pos || args_vector[1] == "startpos") {
        if (!moves_pos) {
            board->set_game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", false);        
            moves_pos = 3;
        }

        for (int i = moves_pos; i < length; i++) {
            board->play_move(uci_to_move(args_vector[i], *board));
        }
    }

    if (args_vector[1] == "display") {
        if (length > 2 && args_vector[2] == "bitboards") 
            board->display_game(true);
        else
            board->display_game();
    }
}

void Interface::go(std::vector<std::string>& args_vector, int length) {
    if (args_vector[1] == "perft") {
        perft_info(*board, std::stoi(args_vector[2]));
        return;
    }

    else if (args_vector[1] == "depth") {
        searcher->start_search(*board, 1000000000, std::stoi(args_vector[2]));
        printf("bestmove %s\n", move_to_uci(searcher->best_move).c_str()); // implement ponder (eventually)
        return;
    }

    bool turn = board->get_turn();
    int time/*, increment*/;
    for (int i = 1; i < length; i++) {
        if (turn == WHITE && args_vector[i] == "wtime")
            time = std::stoi(args_vector[i+1]);

        if (turn == BLACK && args_vector[i] == "btime")
            time = std::stoi(args_vector[i+1]);

        /*if (turn == WHITE && args_vector[i] == "winc")
            increment = args_vector[i+1];

        if (turn == BLACK && args_vector[i] == "binc")
            increment = args_vector[i+1];*/
    }

    if (time) {
        printf("time: %i\n", time);
        searcher->start_search(*board, searcher->get_time_for_move(time));
        printf("bestmove %s\n", move_to_uci(searcher->best_move).c_str());
    }
}
