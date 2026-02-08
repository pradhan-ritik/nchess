#include "board.hpp"

Board::Board(const char* fen) {
    history_pointer = -1;
    for (int i = 0; i < 256; i++) {
        history[i] = EMPTY_HISTORY;
    }

    for (int i = 0; i < 64; i++) 
        board_array[i] = EMPTY;
    
    for (int i = 0; i < 7; i++) 
        pieces[i] = 0ULL;

    for (int i = 0; i < 2; i++) 
        colors[i] = 0ULL;


    turn = false;
    rules = 0u;

    int pos = 0;
    int slash_count = 0;
    char cur;
    bool white_piece;
    bool running = true;
    while (running) {
        cur = *(fen++);
        white_piece = std::isupper(cur);
        cur = std::tolower(cur);
        switch (cur) {
            case 'p':
                if (white_piece) set_piece_on_pos(PAWN, pos^63);
                else set_opposite_color_piece_on_pos(PAWN, pos^63);
                break;

            case 'n':
                if (white_piece) set_piece_on_pos(KNIGHT, pos^63);
                else set_opposite_color_piece_on_pos(KNIGHT, pos^63);
                break;

            case 'b':
                if (white_piece) set_piece_on_pos(BISHOP, pos^63);
                else set_opposite_color_piece_on_pos(BISHOP, pos^63);
                break;

            case 'r':
                if (white_piece) set_piece_on_pos(ROOK, pos^63);
                else set_opposite_color_piece_on_pos(ROOK, pos^63);
                break;

            case 'q':
                if (white_piece) set_piece_on_pos(QUEEN, pos^63);
                else set_opposite_color_piece_on_pos(QUEEN, pos^63);
                break;

            case 'k':
                if (white_piece) set_piece_on_pos(KING, pos^63);
                else set_opposite_color_piece_on_pos(KING, pos^63);
                break;

            case ' ':
                running = false;
                break;

            case '/':
                pos = ++slash_count*8 - 1;
                break;

            default: // number
                pos += cur - 1 - '0';
                break;
        }

        pos++;
    }

    turn = *(fen++) - 'w';

    if (*(++fen) == '-') {
        fen += 2;
    }

    else {
        while (true) {
            cur = *(fen++);
            if (cur == 'K') set_castle(KINGSIDE, WHITE);
            if (cur == 'Q') set_castle(QUEENSIDE, WHITE);
            if (cur == 'k') set_castle(KINGSIDE, BLACK);
            if (cur == 'q') set_castle(QUEENSIDE, BLACK);
            if (cur == ' ') break;
        }
    }

    if (*fen != '-') {
        set_en_pessant(((*fen - 'a')^7) + ((*(++fen)-'0'-1)*8));
    } 
}

void Board::set_game(const char* fen, bool from_interface) {
    fen += from_interface;
    history_pointer = -1;
    for (int i = 0; i < 256; i++) {
        history[i] = EMPTY_HISTORY;
    }

    for (int i = 0; i < 64; i++) 
        board_array[i] = EMPTY;
    
    for (int i = 0; i < 7; i++) 
        pieces[i] = 0ULL;

    for (int i = 0; i < 2; i++) 
        colors[i] = 0ULL;


    turn = false;
    rules = 0u;

    int pos = 0;
    int slash_count = 0;
    char cur;
    bool white_piece;
    bool running = true;
    while (running) {
        cur = *(fen++);
        white_piece = std::isupper(cur);
        cur = std::tolower(cur);
        switch (cur) {
            case 'p':
                if (white_piece) set_piece_on_pos(PAWN, pos^63);
                else set_opposite_color_piece_on_pos(PAWN, pos^63);
                break;

            case 'n':
                if (white_piece) set_piece_on_pos(KNIGHT, pos^63);
                else set_opposite_color_piece_on_pos(KNIGHT, pos^63);
                break;

            case 'b':
                if (white_piece) set_piece_on_pos(BISHOP, pos^63);
                else set_opposite_color_piece_on_pos(BISHOP, pos^63);
                break;

            case 'r':
                if (white_piece) set_piece_on_pos(ROOK, pos^63);
                else set_opposite_color_piece_on_pos(ROOK, pos^63);
                break;

            case 'q':
                if (white_piece) set_piece_on_pos(QUEEN, pos^63);
                else set_opposite_color_piece_on_pos(QUEEN, pos^63);
                break;

            case 'k':
                if (white_piece) set_piece_on_pos(KING, pos^63);
                else set_opposite_color_piece_on_pos(KING, pos^63);
                break;

            case ' ':
                running = false;
                break;

            case '/':
                pos = ++slash_count*8 - 1;
                break;

            default: // number
                pos += cur - 1 - '0';
                break;
        }

        pos++;
    }

    turn = *(fen++) - 'w';

    if (*(++fen) == '-') {
        fen += 2;
    }

    else {
        while (true) {
            cur = *(fen++);
            if (cur == 'K') set_castle(KINGSIDE, WHITE);
            if (cur == 'Q') set_castle(QUEENSIDE, WHITE);
            if (cur == 'k') set_castle(KINGSIDE, BLACK);
            if (cur == 'q') set_castle(QUEENSIDE, BLACK);
            if (cur == ' ') break;
        }
    }

    if (*fen != '-') {
        set_en_pessant(((*fen - 'a')^7) + ((*(++fen)-'0'-1)*8));
    } 
}

void Board::display_game(bool show_bitboards) {
    PIECE piece;
    char cur;
    for (int i = 0; i < 64; i++) {
        if (!file(i)) {
            printf("\n  "); 
            for (int i = 0; i < 8; i++) 
                printf("____"); 
            printf("\n");
        }

        piece = get_piece_on_pos(i^63);
        switch (piece) {
            case PAWN:
                cur = 'P';
                break;
            
            case KNIGHT:
                cur = 'N';
                break;

            case BISHOP:
                cur = 'B';
                break;

            case ROOK:
                cur = 'R';
                break;

            case QUEEN:
                cur = 'Q';
                break;

            case KING:
                cur = 'K';
                break;

            case EMPTY:
                cur = ' ';
                break;
            
            default:
                break;
        }

        if (!is_white_piece(i^63) && cur != ' ') cur += 32; // turns it into lower case
        if (file(i^63) == 7) printf("\n");
        if (!file(i)) printf("%i | ", (rank(i)^7)+1);
        printf("%c | ", cur);
    }

    printf("\n  ");
    for (int i = 0; i < 8; i++) 
        printf("____"); 
    printf("\n    ");
    for (int i = 0; i < 8; i++) 
        printf("%c   ", i+'A'); 
    printf("\n");

    if (!turn) 
        printf("White to move\n");
    else
        printf("Black to move\n");
    

    if (show_bitboards) {
        for (int i = 0; i < 2; i++) {
            print_BB(get_color(PIECE(i)));
        }

        for (int i = 1; i < 7; i++) {
            print_BB(get_piece_bb(PIECE(i)));
        }
    }

    printf("castle rights\n");
    printf("WK: %i\n", get_castle(KINGSIDE, WHITE));
    printf("WQ: %i\n", bool(get_castle(QUEENSIDE, WHITE)));
    printf("BK: %i\n", get_castle(KINGSIDE, BLACK));
    printf("BQ: %i\n", bool(get_castle(QUEENSIDE, BLACK)));
    
    printf("Current En Pessant: %i\n", get_en_pessant());
}

void Board::play_move(Move move) {
    uint16_t crules = rules;
    int en_pessant_position = 0;
    if (is_normal_move(move)) 
        en_pessant_position = _play_normal_move(move);

    else if (is_promotion(move))
        _play_promotion(move);

    else if (is_castle(move))
        _play_castle(move);

    else if (is_en_pessant(move))
        _play_en_pessant(move);

    else {
        printf("Something went wrong in Board::play_move");
    }

    set_en_pessant(en_pessant_position);
    history[++history_pointer] = init_history(move, captured_piece, crules);
    next_turn();
}

int Board::_play_normal_move(Move move) {
    int from_ = from(move);
    int to_ = to(move);
    PIECE piece = board_array[from_];
    clear_pos(from_);
    set_piece_on_pos(piece, to_);
    if (piece == ROOK) {
        if (from_ == get_original_rook_position(KINGSIDE)) {
            remove_castle(KINGSIDE, turn);
        }

        else if (from_ == get_original_rook_position(QUEENSIDE)) {
            remove_castle(QUEENSIDE, turn);
        }
    }

    else if (piece == PAWN && abs(from_ - to_) == 16) {
        return to_ - get_pawn_direction(); // en pessant
    }


    else if (piece == KING) {
        remove_castle(KINGSIDE | QUEENSIDE, turn);
    }

    return 0; // no en pessant
}

void Board::_play_promotion(Move move) {
    clear_pos(from(move));
    set_piece_on_pos(promotion_piece(move), to(move));
}

void Board::_play_castle(Move move) {
    clear_pos(from(move));
    set_piece_on_pos(KING, to(move));
    CASTLE_SIDE side = castle_side(move);
    int rook_position = get_original_rook_position(side);
    clear_pos(rook_position);
    set_piece_on_pos(ROOK, get_castle_position_rook(rook_position, side));
    remove_castle(KINGSIDE | QUEENSIDE, turn);
}

void Board::_play_en_pessant(Move move) {
    int pawn_direction = get_pawn_direction();
    int to_ = to(move);
    clear_pos(to_ - pawn_direction, true/*for other team*/);
    clear_pos(from(move));
    set_piece_on_pos(PAWN, to_);
}

void Board::undo_last_move() {
    next_turn(); // previous turn
    History cur_history = _get_history_and_pop();
    Move last_move = move_of(cur_history);
    PIECE captured = captured_piece_of(cur_history);
    if (is_normal_move(last_move)) 
        _undo_normal_move(last_move, captured);

    else if (is_promotion(last_move))
        _undo_promotion(last_move, captured);

    else if (is_en_pessant(last_move))
        _undo_en_pessant(last_move);

    else if (is_castle(last_move))
        _undo_castle(last_move);

    else 
        printf("something went wrong in Board::undo_move");

    rules = rules_of(cur_history);
}

void Board::_undo_normal_move(Move move, PIECE captured) {
    if (debug) {
        printf("MOVE: %s\n", move_to_uci(move).c_str());
    }
    int from_ = from(move); 
    int to_ = to(move);
    _undo_set_piece_on_pos(get_piece_on_pos(to_), from_, turn);
    clear_pos(to_);
    if (captured)
        _undo_set_piece_on_pos(captured, to_, other_turn());
} 

void Board::_undo_promotion(Move move, PIECE captured) {
    int from_ = from(move);
    int to_ = to(move);
    clear_pos(to_);
    _undo_set_piece_on_pos(PAWN, from_, turn);
    if (captured)
        _undo_set_piece_on_pos(captured, to_, other_turn());
}

void Board::_undo_en_pessant(Move move) {
    int from_ = from(move);
    int to_ = to(move);
    clear_pos(to_);
    _undo_set_piece_on_pos(PAWN, from_, turn);
    _undo_set_piece_on_pos(PAWN, to_ - get_pawn_direction(), other_turn());
}

void Board::_undo_castle(Move move) {
    int from_ = from(move);
    int to_ = to(move);
    CASTLE_SIDE side = castle_side(move);
    int original_rook_position = get_original_rook_position(side);
    clear_pos(to_);
    clear_pos(get_castle_position_rook(original_rook_position, side));
    _undo_set_piece_on_pos(KING, from_, turn);
    _undo_set_piece_on_pos(ROOK, original_rook_position, turn);
}

bool Board::is_move_pseudo_legal(Move move) {
    int from_ = from(move);
    int to_ = to(move);
    BB bb_from = bb(from_);
    BB bb_to = bb(to_);
    bool is_normal_move_ = is_normal_move(move);
    bool turn = get_turn();
    PIECE piece = get_piece_on_pos(from_);
    BB not_same_team = ~get_color();
    BB empties = get_empties();
    BB other_team = get_other_team_color();

    if (piece == EMPTY || move == NULL_MOVE || (turn == WHITE ? is_black_piece(from_) : !is_black_piece(from_)))
        return false;

    // knight, bishop, rook, and queen are psuedo legal as long as the destination is an attacked square (excluding same team pieces)
    if (piece == KNIGHT) {
        return is_normal_move_ && (bb_to & knight_attacks(bb_from) & not_same_team);
    } 

    if (piece == BISHOP) {
        return is_normal_move_ && (bb_to & bishop_attacks(bb_from, empties) & not_same_team);
    }

    if (piece == ROOK) {
        return is_normal_move_ && (bb_to & rook_attacks(bb_from, empties) & not_same_team);
    }

    if (piece == QUEEN) {
        return is_normal_move_ && (bb_to & queen_attacks(bb_from, empties) & not_same_team);
    }

    if (piece == PAWN) {
        if (is_castle(move))
            return false;

        if (is_en_pessant(move)) {
            return get_en_pessant() == to_ && (pawn_attacks(bb_from, turn) & bb_to); 
        } 

        BB forward = pawn_push(bb_from, turn) & empties;
        BB attacks = pawn_attacks(bb_from, turn) & other_team; 
        BB promotion_rank = RANK_1 | RANK_8;

        if (is_promotion(move)) {
            return promotion_rank & ((attacks & other_team) | forward) & bb_to; 
        }
        
        forward |= pawn_push(forward & get_third_pawn_rank(), turn) & empties; 

        return ~promotion_rank & ((attacks & other_team) | forward) & bb_to;
    }

    if (piece == KING) {
        if (is_normal_move_) {
            return king_attacks(bb_from) & not_same_team & bb_to;
        }

        if (!is_castle(move))
            return false;
        
        // Just try to create two possible castle moves, and if one matches, then it is psuedo legal
        if (king_attackers())
            return false;

        bool is_castle_pseudo_legal = false;

        int shift = turn * 56;
        // kingside castle
        if (get_castle(KINGSIDE, turn) && in_BB(empties, (0b110ULL << shift))) {
            is_castle_pseudo_legal |= move == init_move(from_, shift + 1, CASTLE);
        }

        // queenside castle
        if (get_castle(QUEENSIDE, turn) && in_BB(empties, (0b1110000ULL << shift))) {
            is_castle_pseudo_legal |= move == init_move(from_, shift + 5, CASTLE);
        } 

        return is_castle_pseudo_legal;
    }

    printf("Something went wrong in Board::is_move_psuedo_legal()\n");
    return false;
}
