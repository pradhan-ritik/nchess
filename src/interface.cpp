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

Interface::Interface(Board* board) {
    this->board = board;
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
    int length = args.size();

    if (args_vector[0] == "quit") {
        running = false;
    }

    else if (args_vector[0] == "position") {
        position(args_vector, length);
    }

    else if (args_vector[0] == "go") {
        go(args_vector, length);
    }

}

void Interface::position(std::vector<std::string>& args_vector, int length) {
    if (args_vector[1] == "fen") {
        // printf("fen: %s", std::accumulate(args_vector.begin()+2, args_vector.end(), std::string(""), [](std::string& a, std::string &b) {return a + " " + b;}).c_str());
        board->set_game(std::accumulate(args_vector.begin()+2, args_vector.end(), std::string(""), [](std::string& a, std::string &b) {return a + " " + b;}).c_str());
        board->display_game();
    }
}

void Interface::go(std::vector<std::string>& args_vector, int length) {
    if (args_vector[1] == "perft") {
        perft_info(*board, std::stoi(args_vector[2]));
    }
}