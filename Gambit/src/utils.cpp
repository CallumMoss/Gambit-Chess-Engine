#include "Utils.hpp"

// Shifts should only be used for indiviual piece bbs, not entire board bbs. Aka a singular pawn attack.
u64 Utils::shift_up(u64 board) { // Moving the board up a rank (can be used for seeing possible pawn moves for example)
    return board << 8;
}

u64 Utils::shift_down(u64 board) { // Moving the board down a rank
    return board >> 8;
}

u64 Utils::shift_left(u64 board) { // Moving the board left a file
    return board << 1;
}

u64 Utils::shift_right(u64 board) { // Moving the board right a file
    return board >> 1;
}

bool Utils::piece_is_at_square(uint64_t board, int square) {
    return (board & (1ULL << square)) != 0;
}

// Find position on board from the ls1b
// Use this to determine whether there is a piece of same colour on corresponding square (can check if & 1 << square_of_ls1b)

// Credit to https://www.chessprogramming.org/BitScan#DeBruijnMultiplation and Reference: Hacker's Delight by Henry S. Warren, Jr. and ChatGPT
int find_piece_index(u64 bitboard) {
    static const int index64[64] = {
         0,  1, 48,  2, 57, 49, 28,  3,
        61, 58, 50, 42, 38, 29, 17,  4,
        62, 55, 59, 36, 53, 51, 43, 22,
        45, 39, 33, 30, 24, 18, 12,  5,
        63, 47, 56, 27, 60, 41, 37, 16,
        54, 35, 52, 21, 44, 32, 23, 11,
        46, 26, 40, 15, 34, 20, 31, 10,
        25, 14, 19,  9, 13,  8,  7,  6
    };

    const uint64_t debruijn64 = 0x03f79d71b4cb0a89;
    return index64[((bitboard & -bitboard) * debruijn64) >> 58];
}

// Prints the board in bit representation, puts an X where the piece by specifying board center
void Utils::PrintBB(u64 board, int board_center, bool mirrored) {
        std::string output{}, current_line{};
        for(int row{0}; row < 8; ++row)
        {
            for(int col{0}; col < 8;++col)
            {
                if((col + row * 8) == board_center)
                {
                    current_line = mirrored ?   current_line + "X " : "X " + current_line;
                }
                else if(((board >> (col + row * 8))) & 1ull)
                {
                    current_line = mirrored ?   current_line + "1 " : "1 " + current_line ;
                }
                else
                {
                    current_line = mirrored ?   current_line + "0 " : "0 " + current_line;
                }
            }
            current_line += "|" + std::to_string(row + 1) + " \n";
            output = current_line + output;
            current_line = "";
        }
        output += "----------------\n";
        output += mirrored ? "A B C D E F G H" : "H G F E D C B A";
        std::cout << output << "\n\n";
}
