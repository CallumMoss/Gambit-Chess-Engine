#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <array>
#include <vector>

#include "utils.hpp"

enum class Turn: u8 {
    WHITE,
    BLACK
};
class Position { // Game state class
    public:
        Position(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        bool piece_is_at_square(uint64_t board, int square);
        void print_position();
        std::array<char, 64> board_to_char_array(u64 board);
        void print_board(std::array<char, 64> board);

        u64 generate_pawn_attacks(int square);
        u64 get_rook_moves(Position pos, int square);
        std::vector<bb_vector> generate_all_moves(Position pos);
        bb_vector generate_piece_moves(Position pos, Piece type, int square);
        std::vector<u64> extract_piece_moves(u64 attacks);

        Piece get_piece_type(int square);
        std::array<u64, 6> get_pieces();
        std::array<u64, 2> get_colours();
        u64 get_en_passant_target();
        int get_half_move_clock();
        int get_full_move_counter();
        u8 get_castling_rights();
        Turn get_turn();

        u64 get_pawns();
        u64 get_white_pawns();
        u64 get_black_pawns();

        u64 get_knights();
        u64 get_white_knights();
        u64 get_black_knights();

        u64 get_bishops();
        u64 get_white_bishops();
        u64 get_black_bishops();

        u64 get_rooks ();
        u64 get_white_rooks();
        u64 get_black_rooks();

        u64 get_queens ();
        u64 get_white_queen();
        u64 get_black_queen();

        u64 get_kings ();
        u64 get_white_king ();
        u64 get_black_king ();

        u64 get_white_pieces();
        u64 get_black_pieces();

        u64 get_board();

        bool get_wscr(); // returns true if white has short castling rights
        bool get_wlcr();
        bool get_bscr();
        bool get_blcr();

        void set_turn(Turn);
    private:
        // Piece-centric bitboards for storing the position of the pieces by types and colour
        std::array<u64, 6> pieces;
        std::array<u64, 2> colours;
		u64 en_passant_target;
        int half_move_clock; // number of half moves, to test for 50 move rule
		int full_move_counter; // how many moves have been played
        u8 castling_rights; // XXXX-BL-BS-WL-WS, last 4 bits, 0 if cannot castle
        Turn turn;
};

#endif // #ifndef POSITION_HPP