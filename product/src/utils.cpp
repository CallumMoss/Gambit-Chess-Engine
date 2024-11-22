#include "Utils.hpp"
#include "Position.hpp"
#include <algorithm> 
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

// Use this to determine whether there is a piece of same colour on corresponding square (can check if & 1 << square_of_ls1b)
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
void Utils::PrintBB(u64 board, int board_center) {
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

Move Utils::board_notation_to_move(std::string board_notation, Position& pos) {
    u8 src_square = (board_notation[0] - 'a') + (board_notation[1] - '1') * 8;
    u8 dest_square = (board_notation[2] - 'a') + (board_notation[3] - '1') * 8;
    Move_Flag flag;
    if(board_notation.length() == 5) {
        switch(board_notation[4]) {
            case 'q':
                flag = Move_Flag::QUEEN_PROMOTION_FLAG;
                break;
            case 'r':
                flag = Move_Flag::ROOK_PROMOTION_FLAG;
                break;
            case 'b':
                flag = Move_Flag::BISHOP_PROMOTION_FLAG;
                break;
            case 'n':
                flag = Move_Flag::KNIGHT_PROMOTION_FLAG;
                break;
        }
        return Move(src_square, dest_square, flag);
    }
    else {
        return encode_move(pos.get_piece_type_from_square(src_square), src_square, dest_square, pos.get_en_passant_target());
    }
}

// Most Valuable Victim - Least Valuable Aggressor
// Simply heuristic to help sort captures in a way where it is more likely to benefit from the exchange
// Ranges from [4, 49], where the lower the number the higher the priority
int Utils::find_mvv_lva(Piece& victim_type, Piece& attacker_type) { // we give weight towards the victim to ensure it gets sorted first
    return ((value_of_piece_from_type_and_capture_role(victim_type, true) * 10) - value_of_piece_from_type_and_capture_role(attacker_type, false));
}

int Utils::value_of_piece_from_type_and_capture_role(Piece& type, bool is_victim) {
    if(is_victim) {
        switch(type) {
            case Piece::PAWN:
                return 5;
            case Piece::KNIGHT:
                return 4;
            case Piece::BISHOP:
                return 3;
            case Piece::ROOK:
                return 2;
            case Piece::QUEEN:
                return 1;
            default: // king cannot be taken
                std::cerr << "Unexpected piece type of victim: " << type;
                std::exit(-1);
        }
    }
    //else if attacker
    return type + 1; // king is most valuable attacker as it implies that it is free, as he cannot walk into check
}

struct Mvv_lva_log {
    Move move;
    int mvv_lva_score;
};

std::vector<Move> Utils::sort_by_mvv_lva(std::vector<Move>& moves, Position& pos) {
    std::vector<Mvv_lva_log> move_logs;
    // Iterate through each move
    for(Move move : moves) {
        // Check if it's a capture (if the destination has a piece)
        if(!Utils::piece_is_at_square(pos.get_board(), static_cast<int>(move.get_dest_square()))) {
            // Not a capture, assign lowest priority (50)
            Mvv_lva_log move_log = {move, 50};
            move_logs.push_back(move_log);
        } else {
            // It's a capture, calculate MVV-LVA score
            Piece victim = pos.get_piece_type_from_square(move.get_dest_square());
            Piece attacker = pos.get_piece_type_from_square(move.get_src_square());
            int mvv_lva_score = find_mvv_lva(victim, attacker); // This should give a score prioritizing valuable captures

            // Log the move and its score
            Mvv_lva_log move_log = {move, mvv_lva_score};
            move_logs.push_back(move_log);
        }
    }

    // Sort the moves based on MVV-LVA score, higher priority first
    std::sort(move_logs.begin(), move_logs.end(), [](const Mvv_lva_log& a, const Mvv_lva_log& b) {
        return a.mvv_lva_score < b.mvv_lva_score; // lower scores at the front
    });

    // Extract sorted moves from the logs
    std::vector<Move> sorted_moves;
    for(Mvv_lva_log& log : move_logs) {
        sorted_moves.push_back(log.move);
    }
    return sorted_moves;
}



// std::vector<Move> Utils::sort_by_mvv_lva_and_remove_captures(std::vector<Move>& moves, u64 board) {
//     for(Move move : moves) {
//         if(!Utils::piece_is_at_square(move.get_dest_square(), board)) { // if not a capture
//             continue;
//         }
//     }
// }