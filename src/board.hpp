#ifndef BOARD_HPP
#define BOARD_HPP

#include "constants.hpp"
#include "bit_operations.hpp"
#include "piece.hpp"
#include "move.hpp"
#include "movegen.hpp"
#include "history.hpp"
#include "movelist.hpp"


struct Board {
    private:
    History history[512];
    PIECE board_array[64];
    BB pieces[7]; // pieces[EMPTY] is unused
    BB colors[2];
    uint16_t rules; // first 6 bits are last en pessant, next 4 bits are for if king/queen white/black already castled
    PIECE captured_piece;
    uint16_t history_pointer;
    bool turn; // white 0, black 1


    int _play_normal_move(Move move); // returns position of en pessant pawn
    void _play_promotion(Move move);
    void _play_en_pessant(Move move);
    void _play_castle(Move move);

    void _undo_normal_move(Move move, PIECE captured);
    void _undo_promotion(Move move, PIECE captured);
    void _undo_en_pessant(Move move);
    void _undo_castle(Move move);

    // private s/getters
    inline void _undo_set_piece_on_pos(PIECE piece, int pos, bool turn_) { // should not be used outside of undo move
        board_array[pos] = piece;
        set_bit_on(pieces[piece], pos);
        set_bit_on(colors[turn_], pos);
    }

    inline History _get_history_and_pop() {
        History h = history[history_pointer--];
        return h;
    }

    public:
    Board(const char* fen);
    void set_game(const char* fen);
    void display_game(bool show_bitboards=false);
    bool play_move(Move move); // returns true if the move was legal
    void undo_last_move();
    void generate_psuedo_legal_moves(Movelist& movelist);
    void generate_noisy_moves(Movelist& movelist); // noisy moves include captures, and checks
    void generate_quiet_moves(Movelist& movelist);


    // small and quick functions
    inline bool is_move_legal(Move move) {
        bool legal = play_move(move);
        undo_last_move();
        return legal;
    }


    // public s/getters

    inline History get_history() {
        return history[history_pointer];
    }

    inline void set_piece_on_pos(PIECE piece, int pos) {
        captured_piece = board_array[pos];
        // printf("PIECE: %i POS: %i \n", piece, pos);
        board_array[pos] = piece;
        set_bit_on(pieces[piece], pos);
        set_bit_on(colors[turn], pos);
        set_bit_off(pieces[captured_piece], pos);
        set_bit_off(colors[other_turn()], pos);
    }


    inline void set_opposite_color_piece_on_pos(PIECE piece, int pos) { // only used for fen
        next_turn();
        set_piece_on_pos(piece, pos);
        next_turn();
    }

    inline void clear_pos(int pos) {
        PIECE removed = board_array[pos];
        board_array[pos] = EMPTY;
        set_bit_off(pieces[removed], pos);
        set_bit_off(colors[turn], pos);
    }

    inline PIECE get_piece_on_pos(int pos) {
        return board_array[pos];
    }

    inline bool get_turn() {
        return turn;
    }

    inline void next_turn() {
        turn ^= 1u;
    }

    inline bool other_turn() {
        return turn ^ 1u;
    }

    inline BB get_color(bool turn_) {
        return colors[turn_];
    }

    inline BB get_color() {
        return get_color(get_turn());
    }

    inline BB get_other_team_color() {
        return get_color(other_turn());
    }

    inline bool is_black_piece(int pos) {
        return get_bit(get_color(1), pos);
    }

    inline bool is_white_piece(int pos) {
        return !is_black_piece(pos);
    }

    inline void set_piece_bb(PIECE piece, BB bitboard) {
        pieces[piece] = bitboard;
    }

    inline BB get_piece_bb(PIECE piece) {
        return pieces[piece];
    }

    inline BB get_empties() {
        return ~(get_color() | get_other_team_color());
    }

    inline BB get_piece_bb(PIECE piece, bool color) {
        return get_piece_bb(piece) & get_color(color);
    }

    inline void set_en_pessant(int pos) {
        rules &= ~0b111111u;
        rules |= pos;
    }

    inline void clear_en_pessant() {
        rules &= ~0b111111u;;
    }

    inline int get_en_pessant() {
        return rules & 0b111111u;
    }

    inline void set_castle(CASTLE_SIDE side, bool turn_) {
        rules |= side << (6+2*turn_);
    }

    inline void remove_castle(CASTLE_SIDE side, bool turn_) {
        rules &= ~(side << (6+2*turn_)); 
    }

    inline CASTLE_SIDE get_castle(CASTLE_SIDE side, bool turn_) {
        return (rules >> (6+2*turn_)) & side;
    } 

    inline int get_pawn_direction() {
        return 8 - 16*turn; // -8 for black, 8 for white
    }

    inline int get_original_rook_position(CASTLE_SIDE side) {
        return 7*(side-1) + turn * 56;
    }

    inline int get_castle_position_rook(int pos, CASTLE_SIDE side) {
        return pos - 3 + 5 * ((side-1)^1);
    }

    inline int get_king_pos() {
        return lsb(get_piece_bb(KING, turn));
    }

    inline BB square_attackers(BB pos) {
        BB empties = get_empties();
        BB same_team = get_color();
        bool other_team = other_turn();
        BB other_team_queens = get_piece_bb(QUEEN, other_team);
        return ((other_team_queens | get_piece_bb(BISHOP, other_team)) & bishop_attacks(pos, empties)) |
                ((other_team_queens | get_piece_bb(ROOK, other_team)) & rook_attacks(pos, empties)) |
                (pawn_attacks(get_piece_bb(PAWN, pos), same_team) & get_piece_bb(PAWN, other_team)) |
                (knight_attacks(pos) & get_piece_bb(KNIGHT, other_team))
                ;
    }

    inline BB square_attackers(int pos) {
        return square_attackers(bb(pos));
    }

    inline BB king_attackers() {
        return square_attackers(lsb(get_piece_bb(KING, turn)));
    }

    // get the last player's king position and see if it is attacked
    inline bool was_move_legal() {
        next_turn();
        bool was_legal = !king_attackers();
        next_turn();
        return was_legal;
    }

    inline bool is_move_capture(Move move) {
        return get_piece_on_pos(to(move)) || is_en_pessant(move);
    }

    inline BB get_pawn_promotion_rank() {
        return turn ? RANK_2 : RANK_7; // rank 2 for black and rank 7 for white
    }

    inline BB get_third_pawn_rank() {
        return turn ? RANK_6 : RANK_3; // rank 6 for black and rank 3 for white
    }
};

#endif // BOARD_HPP