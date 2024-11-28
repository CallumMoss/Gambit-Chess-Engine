#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <array>
#include <vector>

#include "Types.hpp"
#include "Magics.hpp"
#include "utils.hpp"
#include "Timer.hpp"
#include "Zobrist.hpp"

class Position
{ // Game state class
    public:
        Position(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        void print_position();
        std::array<char, 64> board_to_char_array(u64 board);
        void print_board(std::array<char, 64> board);
        void compute_zobrist_key();
        void recompute_zobrist_key();

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

        // Applying moves and legality checks
        void make_move(Move& move);
        void handle_move(const Piece& src_square_type, const Piece& dest_square_type, const Piece& captured_piece_type, const u8& src_square, const u8& dest_square);
        void handle_en_passant(const u8& src_square, const u8& dest_square);
        void handle_castling(const u8& src_square, const u8& dest_square);
        bool legality_check(Move& move) const;
        bool in_check() const;
        u64 split_perft(int current_depth, const int& desired_depth, const bool& output_split, Move& last_move);

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
        //Colour get_colour_from_square(u8 square);

        u64 get_zobrist_key() const;

        void remove_wscr();
        void remove_wlcr();
        void remove_bscr();
        void remove_blcr();

        int get_colour_value_from_square(u8 square);

        void set_turn(Turn);
        void set_pieces_and_colours(const Piece& moved_piece_type, const Piece& captured_piece_type, const Piece& promoted_piece_type, const Turn& turn, const u8& src_square, const u8& dest_square, bool is_en_passant);
        bool equals(const Position& posb) const;
        std::string equals_with_debugging(const Position& posb) const;
        std::string zobrist_equals_with_debugging(const u64& zobrist_key_b) const;

    private:
        // Piece-centric bitboards for storing the position of the pieces by types and colour
        std::array<u64, 6> pieces;
        std::array<u64, 2> colours;
		u8 en_passant_target; // square the pawn travels to in order to take the double pushed pawn
        u8 half_move_clock; // number of half moves, to test for 50 move rule
		u8 full_move_counter; // how many moves have been played
        u8 castling_rights; // XXXX-BL-BS-WL-WS, last 4 bits, 0 if cannot castle
        Turn turn;

        u64 zobrist_key = 0ULL; // hash value representing a position (discluding half clock, full move)
};

#endif // #ifndef POSITION_HPP