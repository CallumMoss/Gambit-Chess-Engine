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
struct MagicEntry {
    u64 mask;
    u64 magic_number;
    int index_bits; // Could probably use a u8 for smaller data type but this keeps it simple
};

struct Final_Magic {
    bb_vector table;
    MagicEntry magic;
};

class Position { // Game state class
    public:
        Position(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        bool piece_is_at_square(uint64_t board, int square);
        void print_position();
        std::array<char, 64> board_to_char_array(u64 board);
        void print_board(std::array<char, 64> board);

        u64 generate_pawn_attacks(int square);
        u64 get_relevant_rook_blockers(int square, u64 attacks);
        u64 get_blockers(int square);
        bb_vector get_blocker_combinations(u64 blockers);
        bb_vector get_attacks_after_blockers(int square, u64 attacks, bb_vector blocking_squares);
        u64 pseudo_legalise_rook_attacks_slow(int square, u64 attacks);
        bb_vector pseudo_legalise_rook_attacks(int square, u64 attacks);
        std::vector<u64> generate_piece_moves(int square, Piece type);
        std::vector<u64> extract_piece_moves(u64 attacks);
        Final_Magic find_magic(int square);
        bb_vector create_magic_table(MagicEntry magic, int square);
        u64 get_relevant_blocker_squares(int square);

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