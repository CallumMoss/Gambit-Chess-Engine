#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <array>
#include <vector>

#include "utils.hpp"
#include "Timer.hpp"

enum class Turn: u8 {
    WHITE,
    BLACK
};

struct PV {
    int num_of_moves;
    std::vector<Move> moves;
};

class Position { // Game state class
    public:
        Position(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        void print_position();
        std::array<char, 64> board_to_char_array(u64 board);
        void print_board(std::array<char, 64> board);

        // Move Gen:
        u64 get_pawn_attacks(u8 square) const;
        u64 get_knight_moves(u8 square) const;
        u64 get_king_moves(u8 square) const;
        u64 get_bishop_moves(u8 square) const;
        u64 get_rook_moves(u8 square) const;
        u64 get_queen_moves(u8 square) const;
        std::vector<Move> generate_all_moves() const;
        u64 generate_piece_attacks(Piece type, u8 square) const;
        std::vector<Move> generate_piece_moves(Piece type, u8 square) const;
        std::vector<u64> extract_piece_moves(u64 attacks) const;
        std::vector<Move> bb_to_move_list(Piece type, u8 square, u64 attacks) const;
        void make_move(Move& move);
        bool legality_check(Move& move) const;
        u64 split_perft(int current_depth, const int& desired_depth, const bool& output_split);

        // Getters and Setters:
        Piece get_piece_type_from_square(u8 square) const;
        std::array<u64, 6> get_pieces() const;
        std::array<u64, 2> get_colours() const;
        u64 get_en_passant_target() const;
        u8 get_half_move_clock() const;
        u8 get_full_move_counter() const;
        u8 get_castling_rights() const;
        Turn get_turn() const;
        Turn get_opp_turn() const;

        u64 get_pawns() const;
        u64 get_white_pawns() const;
        u64 get_black_pawns() const;

        u64 get_knights() const;
        u64 get_white_knights() const;
        u64 get_black_knights() const;

        u64 get_bishops() const;
        u64 get_white_bishops() const;
        u64 get_black_bishops() const;

        u64 get_rooks () const;
        u64 get_white_rooks() const;
        u64 get_black_rooks() const;

        u64 get_queens () const;
        u64 get_white_queen() const;
        u64 get_black_queen() const;

        u64 get_kings () const;
        u64 get_white_king () const;
        u64 get_black_king () const;

        u64 get_white_pieces() const;
        u64 get_black_pieces() const;

        u64 get_pieces_from_current_turn() const;
        u64 get_board() const;

        bool get_wscr() const; // returns true if white has short castling rights
        bool get_wlcr() const;
        bool get_bscr() const;
        bool get_blcr() const;

        void remove_wscr();
        void remove_wlcr();
        void remove_bscr();
        void remove_blcr();

        void set_turn(Turn);
        void set_pieces_and_colours(const Piece& moved_piece_type, const Piece& captured_piece_type, const Piece& promoted_piece_type, const Turn& turn, const u8& src_square, const u8& dest_square, bool is_en_passant);

    private:
        // Piece-centric bitboards for storing the position of the pieces by types and colour
        std::array<u64, 6> pieces;
        std::array<u64, 2> colours;
		u8 en_passant_target;
        u8 half_move_clock; // number of half moves, to test for 50 move rule
		u8 full_move_counter; // how many moves have been played
        u8 castling_rights; // XXXX-BL-BS-WL-WS, last 4 bits, 0 if cannot castle
        Turn turn;

        // //if pawn starting square is on the right rank and destination is next to pawn of opposing colour on the right rank

        // Move List
        // If this move causes en passant next move, turn on the en passant flag 

        // 8 - 15 source squares
        // en_passant ranks: 48 - 55,



        // check if a move causes en passant, set flag to that square

        // check if en passant is an option currently available
        // if en passant flag is true, check its target square, check if there is a pawn on a square that could target it
        
        // for example, for a given colour and target square, possible squares a pawn must be on would be +7 and +9 of the target square
        // then we & with the pawns

};

#endif // #ifndef POSITION_HPP