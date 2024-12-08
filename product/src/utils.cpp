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

bool Utils::piece_is_at_square(u64 board, int square) { // Checking whether a piece is at a given square on the provided board (can be added as any type of board, the entire or just one piece etc.)
    return (board & (1ULL << square)) != 0;
}

int Utils::count_number_of_1bs(u64 board) {
    return __builtin_popcountll(board);
}

// Finds the square index of the ls1b
// Credit to https://www.chessprogramming.org/BitScan#DeBruijnMultiplation and Reference: Hacker's Delight by Henry S. Warren, Jr. and ChatGPT
u8 Utils::find_piece_index(u64 bitboard) {
    if(bitboard == 0ULL) {
        return 64; // Not applicable
    }
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
    return (u8)index64[((bitboard & -bitboard) * debruijn64) >> 58];
}

// Prints the board in bit representation, puts an X where the piece by specifying board center
void Utils::print_bb(u64 board, int board_center) {
        bool mirrored = true;
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

u64 Utils::clear_bit(u64 board, int index) {
    return board &= ~(1ULL << index);
}

std::string Utils::move_to_board_notation(Move move) {
    std::string board_notation = "";
    board_notation += 'a' + (move.get_src_square() % 8);  // Calculate file ('a' to 'h')
    board_notation += '1' + (move.get_src_square() / 8);  // Calculate rank ('1' to '8')

    board_notation += 'a' + (move.get_dest_square() % 8);  // Calculate file ('a' to 'h')
    board_notation += '1' + (move.get_dest_square() / 8);  // Calculate rank ('1' to '8')
    switch(move.get_flag()) {
        case KNIGHT_PROMOTION_FLAG:
            board_notation += 'n';
            break;
        case BISHOP_PROMOTION_FLAG:
            board_notation += 'b';
            break;
        case ROOK_PROMOTION_FLAG:
            board_notation += 'r';
            break;
        case QUEEN_PROMOTION_FLAG:
            board_notation += 'q';
            break;
        default:
            break;
    }
    return board_notation;  // Construct string using file and rank
}

Move Utils::encode_move(Piece type, u8 src_square, u8 dest_square, u8 en_passant_target) {
    Move_Flag flag = Move_Flag::NULL_FLAG;

    if (type == Piece::PAWN) {
        if((src_square == dest_square + 16) || (src_square == dest_square - 16)) { // if is pawn and has moved twice
            flag = Move_Flag::PAWN_TWO_FORWARD_FLAG; // this move gives the option of en_passant for next move
        }
        else if(dest_square == en_passant_target) {
            flag = Move_Flag::EN_PASSANT_FLAG;
        } // then en passant has occured
        else {
            flag = Move_Flag::PAWN_FLAG;
        }
    }
    else if(type == Piece::KING) {
        if((src_square == dest_square + 2) || (src_square == dest_square - 2)) { // castling has occured
            flag = Move_Flag::CASTLING_FLAG;
        }
        else {
            flag = Move_Flag::KING_FLAG;
        }
    }
    else {
        switch(type) {
            case(Piece::KNIGHT):
                flag = Move_Flag::KNIGHT_FLAG;
                break;
            case(Piece::BISHOP):
                flag = Move_Flag::BISHOP_FLAG;
                break;
            case(Piece::ROOK):
                flag = Move_Flag::ROOK_FLAG;
                break;
            case(Piece::QUEEN):
                flag = Move_Flag::QUEEN_FLAG;
                break;
            default: // if invalid piece type
                flag = Move_Flag::NULL_FLAG;
                break;
        }
    }
    return Move(src_square, dest_square, flag);
}

std::string Utils::square_to_board_notation(u8 square) {
    std::string board_notation = "";
    board_notation += 'a' + (square % 8);  // Calculate file ('a' to 'h')
    board_notation += '1' + (square / 8);  // Calculate rank ('1' to '8')
    return board_notation;
}