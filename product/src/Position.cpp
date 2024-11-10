#include "Position.hpp"
#include "Magics.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>

Position::Position(const std::string& fen):
    pieces(),
        colours(),
            en_passant_target(), // Initializes to the default value. Can alternatively put whatever value here i want.
                half_move_clock(),
                    full_move_counter(),
                        castling_rights(),
                            turn(Turn::WHITE){

		// turn: 0 for white, 1 for black
        // castling_rights: XXXX-BL-BS-WL-WS, last 4 bits, 0 if cannot castle
		// en_passant_target: the target square of en passant
	    // half_move_clock: number of half moves, to test for 50 move rule
		// full_move_counter: how many moves have been played

    // Create a stringstream from the input string
    std::istringstream iss(fen);
    std::vector<std::string> words;
    // Parse the string based on space (' ')
    std::string word;
    while (std::getline(iss, word, ' ')) {
        words.push_back(word); // pushes word to the back
    }

    int rank = 7, file = 0;
    for (char c : words[0]) {

        if (c == '/') {
            // this splits each rank
            rank--;
            file = 0;
        }
        else if (isdigit(c)) {
            file += (c - '0'); // skips empty files of this amount
        }
        else {
            uint64_t square = 1ULL << (rank * 8 + file);
            // shifts 1 bit, to represent there is a piece on the square, across the bitboard's bitstring
            // For example, c4 in LERF is 26. So you want to push a 1 onto the 26th bit of the bitstring
            switch (c) {
                using namespace Utils;
                // Bit boards are all 0s, by using or, the 1 is added in.
                // Need to use or instead of equals as after the first pawn is detected,
                // square wont remember where the 1 was originally for the first pawn
                case 'P': pieces[Piece::PAWN] |= square; colours[Colour::WHITE] |= square; break;
                case 'N': pieces[Piece::KNIGHT] |= square; colours[Colour::WHITE] |= square; break;
                case 'B': pieces[Piece::BISHOP] |= square; colours[Colour::WHITE] |= square; break;
                case 'R': pieces[Piece::ROOK] |= square; colours[Colour::WHITE] |= square; break;
                case 'Q': pieces[Piece::QUEEN] |= square; colours[Colour::WHITE] |= square; break;
                case 'K': pieces[Piece::KING] |= square; colours[Colour::WHITE] |= square; break;
                case 'p': pieces[Piece::PAWN] |= square; colours[Colour::BLACK] |= square; break;
                case 'n': pieces[Piece::KNIGHT] |= square; colours[Colour::BLACK] |= square; break;
                case 'b': pieces[Piece::BISHOP] |= square; colours[Colour::BLACK] |= square; break;
                case 'r': pieces[Piece::ROOK] |= square; colours[Colour::BLACK] |= square; break;
                case 'q': pieces[Piece::QUEEN] |= square; colours[Colour::BLACK] |= square; break;
                case 'k': pieces[Piece::KING] |= square; colours[Colour::BLACK] |= square; break;
            }
            file++;
        }
    }

    char file_letter; // declared out of switch statement
    int file_number;

    for (int i = 1; i < 6; i++) {
        std::string word = words[i];
        switch (i) {
            case 1: // which colour to move
                if (word[0] == 'w') { turn = Turn::WHITE; }
                else { turn = Turn::BLACK; }
                break;
            case 2: // which colours have castling rights
                for (char c : word) {
                    switch (c) {
                        case('-'):
                            castling_rights = 0b0;
                            break;
                        case('K'):
                            castling_rights |= 0b1;
                            break;
                        case('Q'):
                            castling_rights |= 0b1 << 1;
                            break;
                        case('k'):
                            castling_rights |= 0b1 << 2;
                            break;
                        case('q'):
                            castling_rights |= 0b1 << 3;
                            break;
                    }

                }
                break;
            case 3: // which squares are being targetted with en passant
                file_letter = word[0];
                if (file_letter == '-') { en_passant_target = 64ULL; break; }
                else if (file_letter == 'a') { file_number = 0; }
                else if (file_letter == 'b') { file_number = 1; }
                else if (file_letter == 'c') { file_number = 2; }
                else if (file_letter == 'd') { file_number = 3; }
                else if (file_letter == 'e') { file_number = 4; }
                else if (file_letter == 'f') { file_number = 5; }
                else if (file_letter == 'g') { file_number = 6; }
                else { file_number = 7; } // if == 'h'

                en_passant_target = 1ULL << ((word[1] - 1) * 8 + file_number);
                break;
            case 4: // how many half moves have been played
                half_move_clock = std::stoi(word); // string to integer
                break;
            case 5: // how many moves have been played
                full_move_counter = std::stoi(word);
                break;
        }
    }
}

// Changes board values to a given position, then prints it
void Position::print_position() {
    std::array<char, 64> board;
    // Get letter values for all pieces in the position/
    for (int i = 0; i < 64; i++) {
        if (Utils::piece_is_at_square(get_white_pawns(), i)) { board[i] = 'P'; }
        else if (Utils::piece_is_at_square(get_white_knights(), i)) { board[i] = 'N'; }
        else if (Utils::piece_is_at_square(get_white_bishops(), i)) { board[i] = 'B'; }
        else if (Utils::piece_is_at_square(get_white_rooks(), i)) { board[i] = 'R'; }
        else if (Utils::piece_is_at_square(get_white_queen(), i)) { board[i] = 'Q'; }
        else if (Utils::piece_is_at_square(get_white_king(), i)) { board[i] = 'K'; }
        else if (Utils::piece_is_at_square(get_black_pawns(), i)) { board[i] = 'p'; }
        else if (Utils::piece_is_at_square(get_black_knights(), i)) { board[i] = 'n'; }
        else if (Utils::piece_is_at_square(get_black_bishops(), i)) { board[i] = 'b';}
        else if (Utils::piece_is_at_square(get_black_rooks(), i)) { board[i] = 'r'; }
        else if (Utils::piece_is_at_square(get_black_queen(), i)) { board[i] = 'q'; }
        else if (Utils::piece_is_at_square(get_black_king(), i)) { board[i] = 'k'; }
        else { board[i] = ' '; }
    }
    print_board(board);
}

// Converts the board state to a character array, which can be used by the print board function.
std::array<char, 64> Position::board_to_char_array(u64 board) {
    std::array<char, 64> char_array;
    for (int i = 0; i < 64; i++) {
        if (Utils::piece_is_at_square(board, i)) { char_array[i] = '0'; }
        else { char_array[i] = '+'; }
    }
    return char_array;
}
void Position::print_board(std::array<char, 64> board) {
    // Can be used in conjunction with board_to_char_array to print a given board.
    std::string rank = "";
    std::cout << "  -------------------------------" << std::endl;
    for(int j = 64; j > 0; j--) {
        if(j % 8 == 0 && j != 64) {
            std::cout << " | " + rank << (j/8) + 1 << std::endl << "  -------------------------------" << std::endl;
            rank = "";
        }
        rank = std::string(1, board[j - 1]) + " | " + rank; // - 1 used so i can use 64 instead of 63
    }
    std::cout << " | " + rank << 1 << std::endl << "  -------------------------------" << std::endl;
    std::cout << "   A   B   C   D   E   F   G   H  " << std::endl << std::endl;
}

u64 Position::get_pawn_attacks(u8 square) {
    u64 attacks = 0ULL;
    if(get_turn() == Turn::WHITE) {
        attacks = Utils::WHITE_PAWN_ATTACKS[square];
        attacks &= ~get_white_pieces();
        if(square + 8 < 64 && Utils::piece_is_at_square(get_board(), square + 8)) { // if something directly infront and is on the board, cant move forward at all
            // if something is infront then clear forward movement
            attacks &= ~(1ULL << square + 8);
        }
        if(square >= 8 && square <= 15 && Utils::piece_is_at_square(get_board(), square + 16)) { // if can move twice but something is 2 spaces in front, we cant move twice
            attacks &= ~(1ULL << square + 16);
        }
    }
    else {
        attacks = Utils::BLACK_PAWN_ATTACKS[square];
        attacks &= ~get_black_pieces();
        if(square - 8 >= 0 && Utils::piece_is_at_square(get_board(), square - 8)) { // if something directly infront and is on the board, cant move forward at all
            // if something is infront then clear forward movement
            attacks &= ~(1ULL << square - 8);
        }
        if(square >= 48 && square <= 55 && Utils::piece_is_at_square(get_board(), square - 16)) { // if can move twice but something is 2 spaces in front, we cant move twice
            attacks &= ~(1ULL << square - 16);
        }
    }

    if(en_passant_target != 64) { // if there is an en passant, add it
        attacks |= 1ULL << en_passant_target;
    }
    return attacks;
}

u64 Position::get_knight_moves(u8 square) {
    return Utils::KNIGHT_ATTACKS[square];
}

u64 Position::get_king_moves(u8 square) {
    u64 moves = Utils::KING_ATTACKS[square];

    // Castling: (rooks move is handled thanks to flags later) | Can check if legal later (like castling flags and checks)
    if(turn == Turn::WHITE) {
        if(square == 4) { // if on starting square
            u64 board = get_board();
            if((Utils::piece_is_at_square(5, board) == Piece::INVALID) && (Utils::piece_is_at_square(6, board) == Piece::INVALID)) { // if clear space between
                moves |= 1ULL << 6; // add short castling
            }
            else if((Utils::piece_is_at_square(3, board) == Piece::INVALID) && (Utils::piece_is_at_square(2, board) == Piece::INVALID) && (Utils::piece_is_at_square(1, board) == Piece::INVALID)) {
                moves |= 1ULL << 2; // long
            }
        }
    }
    else {
        if(square == 60) {
            u64 board = get_board();
            if((Utils::piece_is_at_square(61, board) == Piece::INVALID) && (Utils::piece_is_at_square(62, board) == Piece::INVALID)) { // if clear space between
                moves |= 1ULL << 62; // add short castling
            }
            else if((Utils::piece_is_at_square(59, board) == Piece::INVALID) && (Utils::piece_is_at_square(58, board) == Piece::INVALID) && (Utils::piece_is_at_square(57, board) == Piece::INVALID)) {
                moves |= 1ULL << 58; // long
            }
        }
    }

    return moves;
}

// Gets the piece type of a piece on a given square
Piece Position::get_piece_type_from_square(u8 square) {
    u64 piece_pos = 1ULL << square;
    if(get_pawns() & piece_pos) {return Piece::PAWN;}
    else if (get_knights() & piece_pos) {return Piece::KNIGHT;}
    else if (get_bishops() & piece_pos) {return Piece::BISHOP;}
    else if (get_rooks() & piece_pos) {return Piece::ROOK;}
    else if (get_queens() & piece_pos) {return Piece::QUEEN;}
    else if (get_kings() & piece_pos) {return Piece::KING;}
    else {return Piece::INVALID;} // If no piece on square
}


// Should probably be private
 std::vector<u64> Position::extract_piece_moves(u64 attacks) {
    bb_vector attacks_vector;  // Use a vector for dynamic storage
    while (attacks) {
        // Get the least significant 1 bit (LS1B)
        u64 ls1b = attacks & -attacks;
        // Clear the LS1B from the bitboard
        attacks &= attacks - 1; // Flips every number to the right and itself, therefore new number will always be different to original, so they will be 0
        // Add this move to the array
        if (ls1b != 0) {
            attacks_vector.push_back(ls1b);  // Add the move to the vector
        }
    }
    return attacks_vector;
}

std::vector<Move> Position::generate_all_moves() // uses the board state to generate all moves for a given colour 
{
    std::vector<Move> moves;
    u64 pieces = get_pieces_from_current_turn();
    std::vector<Move> square_moves;
    std::vector<Move> empty_vector; // no moves for this square as the player whos turn it is does not have a piece on it
    for(u8 square = 0; square < 64; square++) {
        if(1ULL << square & pieces) { // if the person whos turn it is has a piece on this square, gen its moves
            Piece piece_type = get_piece_type_from_square(square);
            square_moves = generate_piece_moves(piece_type, square);
            for(Move move : square_moves) {
                moves.push_back(move);
            }
        }
    }
    return moves;
}

u64 Position::get_rook_moves(u8 square) {
    // Returns the attack mask of a rook on a given square
    bb_vector attacks_on_square = rook_magics_table[square];
    MagicEntry magic;
    magic.magic_number = rook_magics[square];
    magic.mask = Magics::get_relevant_blocker_squares(Piece::ROOK, square);
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);
    u64 blockers = Magics::get_blockers(Piece::ROOK, square, get_board());
    u64 attacks = attacks_on_square[Magics::get_magic_index(magic, blockers)];
    if(get_turn() == Turn::WHITE) {
        return attacks & ~get_white_pieces();
    }
    return attacks & ~get_black_pieces();
}

u64 Position::get_bishop_moves(u8 square) {
    // Returns the long table of moves for a rook on that square. To choose which ones apply, you should return 
    bb_vector attacks_on_square = bishop_magics_table[square];
    MagicEntry magic;
    magic.magic_number = bishop_magics[square];
    magic.mask = Magics::get_relevant_blocker_squares(Piece::BISHOP, square);
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);
    u64 blockers = Magics::get_blockers(Piece::BISHOP, square, get_board());
    u64 attacks = attacks_on_square[Magics::get_magic_index(magic, blockers)];
    if(get_turn() == Turn::WHITE) {
        return attacks & ~get_white_pieces();
    }
    return attacks & ~get_black_pieces();
}

u64 Position::get_queen_moves(u8 square) {
    // Returns the attack mask of a queen on a given square
    // Does this by glueing a rook and bishop on the same square together
    u64 attacks = get_rook_moves(square) | get_bishop_moves(square);
    if(get_turn() == Turn::WHITE) {
        return attacks & ~get_white_pieces();
    }
    return attacks & ~get_black_pieces();
}

std::vector<Move> Position::generate_piece_moves(Piece type, u8 square) { // will be used when searching for both sides, so will use Turn.
    u64 attacks = 0ULL;
    u64 blockers = 0ULL;
    switch(type) {
        case Piece::PAWN:
            attacks = get_pawn_attacks(square);
            return bb_to_move_list(type, square, attacks); // skip check for piece colours as already handled.
        case Piece::KNIGHT:
            attacks = Utils::KNIGHT_ATTACKS[square];
            break;
        case Piece::BISHOP:
            attacks = get_bishop_moves(square);
            break;
        case Piece::ROOK:
            attacks = get_rook_moves(square);
            break;
        case Piece::QUEEN:
            attacks = get_queen_moves(square);
            break;
        case Piece::KING:
            attacks = Utils::KING_ATTACKS[square];
            break;
        case Piece::INVALID:
            attacks = 0;
            break;
    }
    // if(turn == Turn::WHITE) {
    //     attacks = attacks & ~get_white_pieces();
    // }
    // else {
    //     attacks = attacks & ~get_black_pieces();
    // }

    return bb_to_move_list(type, square, attacks);

    //return extract_piece_moves(attacks);
}

std::vector<Move> Position::bb_to_move_list(Piece type, u8 src_square, u64 attacks) {
    std::vector<Move> move_list;
    u8 dest_square;
    std::vector<u64> bb_list = extract_piece_moves(attacks);
    for(u64 attack : bb_list) {
        dest_square = Utils::find_piece_index(attack);
        if(type == Piece::PAWN) {
            if((dest_square >=56 && dest_square < 64) || (dest_square >= 0 && dest_square < 8)) { // promotion has occured
            // We dont care about colour as pawns cant get to their own end rank
                move_list.push_back(Move(src_square, dest_square, Move_Flag::KNIGHT_PROMOTION_FLAG));
                move_list.push_back(Move(src_square, dest_square, Move_Flag::BISHOP_PROMOTION_FLAG));
                move_list.push_back(Move(src_square, dest_square, Move_Flag::ROOK_PROMOTION_FLAG));
                move_list.push_back(Move(src_square, dest_square, Move_Flag::QUEEN_PROMOTION_FLAG));
                continue;
            }
        }
        move_list.push_back(encode_move(type, src_square, dest_square));
    }
    return move_list;
}

Move Position::encode_move(Piece type, u8 src_square, u8 dest_square) {
    Move_Flag flag = Move_Flag::NULL_FLAG;

    if (type == Piece::PAWN) {
        if((src_square == dest_square + 16) || (src_square == dest_square - 16)) { // if is pawn and has moved twice
            flag = Move_Flag::PAWN_TWO_FORWARD_FLAG; // this move gives the option of en_passant for next move
        }
    }
    else if(type == Piece::KING) {
        if((src_square == dest_square + 2) || (src_square == dest_square - 2)) { // castling has occured
            flag = Move_Flag::CASTLING_FLAG;
        }
    }
    else {
        switch(type) {
            case(Piece::PAWN):
                flag = Move_Flag::PAWN_FLAG;
                break;
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
            case(Piece::KING):
                flag = Move_Flag::KING_FLAG;
                break;
            default: // if invalid piece type
                flag = Move_Flag::NULL_FLAG;
                break;
        }
    }
    return Move(src_square, dest_square, flag);
}

bool Position::legality_check() {

    // If move type is castling, check if king is in check currently or will be
    // Also check if the squares between the king and rook are attacked

    // Check if king is in check
    return true;
}

// Takes a reference to a position where we apply a move to it.
// Find best move will check if its legal. If it isn't, it will undo the move by making it equal to the original pos
void Position::copy_make(Move move, Position& pos) // simpler than make, unmake but slightly slower.
{
    // std::array<u64, 6> pieces;
    // std::array<u64, 2> colours;
    // u8 en_passant_target = 64; // 64 if not applicable
    // u8 half_move_clock; // number of half moves, to test for 50 move rule
    // u8 full_move_counter; // how many moves have been played
    // u8 castling_rights; // XXXX-BL-BS-WL-WS, last 4 bits, 0 if cannot castle
    // Turn turn;

    // Decode Move:
    Move_Flag flag = move.get_flag();
    Piece piece_type = move.get_piece_type_from_flag();
    u8 src_square = move.get_src_square();
    u8 dest_square = move.get_dest_square();
    Piece captured_piece_type = get_piece_type_from_square(dest_square);
    Piece promoted_piece_type = Piece::INVALID;
    half_move_clock++;
    full_move_counter = half_move_clock / 2; // ints floor divide in C++

    // Adapting for special flags
    switch(flag) {
        case PAWN_TWO_FORWARD_FLAG:
            pos.en_passant_target = dest_square;
            break;
        case EN_PASSANT_FLAG:

            set_pieces_and_colours(Piece::PAWN, Piece::PAWN, Piece::INVALID, turn, src_square, dest_square, true);
             // if this flag, then the dest square and target square are different.
            break;

        case KNIGHT_PROMOTION_FLAG:
            promoted_piece_type = Piece::KNIGHT;
            break;
        case BISHOP_PROMOTION_FLAG:
            promoted_piece_type = Piece::BISHOP;
            break;
        case ROOK_PROMOTION_FLAG:
            promoted_piece_type = Piece::ROOK;
            break;
        case QUEEN_PROMOTION_FLAG:
            promoted_piece_type = Piece::QUEEN;
            break;
        case CASTLING_FLAG:
            // remove corresponding castling rights and play rook portion. After this it will move king as if normal move.
            if(turn == Turn::WHITE) {
                if(dest_square == 6) { // has short castled
                    castling_rights &= ~(1ULL);
                    set_pieces_and_colours(Piece::ROOK, Piece::INVALID, Piece::INVALID, turn, 7, 5, false);
                }
                else { // has long castled
                    castling_rights &= ~(1ULL << 1);
                    set_pieces_and_colours(Piece::ROOK, Piece::INVALID, Piece::INVALID, turn, 0, 3, false);
                }
            }
            else {
                if(dest_square == 62) { // has short castled
                    castling_rights &= ~(1ULL << 2);
                    set_pieces_and_colours(Piece::ROOK, Piece::INVALID, Piece::INVALID, turn, 63, 61, false);
                }
                else { // has long castled
                    castling_rights &= ~(1ULL << 3);
                    set_pieces_and_colours(Piece::ROOK, Piece::INVALID, Piece::INVALID, turn, 56, 59, false);
                }
            }
            break;
    }
    // Update pieces and colours.
    // Does this if it isnt an en passant. Other specials make use of this, for example castling plays this as a rook move previously
    if (flag != EN_PASSANT_FLAG) {
        set_pieces_and_colours(piece_type, captured_piece_type, promoted_piece_type, turn, src_square, dest_square, false);
    }

    if ((en_passant_target != Utils::NULL_EN_PASSANT) && (flag != PAWN_TWO_FORWARD_FLAG)) {en_passant_target = Utils::NULL_EN_PASSANT;} // reset en passant if double push hasnt been played

}

// Uses reference for data type larger than u64
const std::array<u64, 6>& Position::get_pieces() { return pieces; }
const std::array<u64, 2>& Position::get_colours() { return colours; }
u64 Position::get_en_passant_target() { return en_passant_target; }
u8 Position::get_half_move_clock() { return half_move_clock; }
u8 Position::get_full_move_counter() { return full_move_counter; }
u8 Position::get_castling_rights() { return castling_rights; }
Turn Position::get_turn() { return turn; }
Turn Position::get_opp_turn() {
    if(turn == Turn::WHITE) {
        return Turn::BLACK;
    }
    return Turn::WHITE;
}

u64 Position::get_pawns() { return pieces[Piece::PAWN]; }
u64 Position::get_white_pawns() { return pieces[Piece::PAWN] & colours[Colour::WHITE]; }
u64 Position::get_black_pawns() { return pieces[Piece::PAWN] & colours[Colour::BLACK]; }

u64 Position::get_knights() { return pieces[Piece::KNIGHT]; }
u64 Position::get_white_knights() { return pieces[Piece::KNIGHT] & colours[Colour::WHITE]; }
u64 Position::get_black_knights() { return pieces[Piece::KNIGHT] & colours[Colour::BLACK]; }

u64 Position::get_bishops() { return pieces[Piece::BISHOP];}
u64 Position::get_white_bishops() { return pieces[Piece::BISHOP] & colours[Colour::WHITE]; }
u64 Position::get_black_bishops() { return pieces[Piece::BISHOP] & colours[Colour::BLACK]; }

u64 Position::get_rooks() { return pieces[Piece::ROOK]; }
u64 Position::get_white_rooks() { return pieces[Piece::ROOK] & colours[Colour::WHITE]; }
u64 Position::get_black_rooks() { return pieces[Piece::ROOK] & colours[Colour::BLACK]; }

u64 Position::get_queens() { return pieces[Piece::QUEEN];}
u64 Position::get_white_queen() { return pieces[Piece::QUEEN] & colours[Colour::WHITE]; }
u64 Position::get_black_queen() { return pieces[Piece::QUEEN] & colours[Colour::BLACK]; }

u64 Position::get_kings() { return pieces[Piece::KING]; }
u64 Position::get_white_king() { return pieces[Piece::KING] & colours[Colour::WHITE]; }
u64 Position::get_black_king() { return pieces[Piece::KING] & colours[Colour::BLACK]; }

u64 Position::get_white_pieces() { return colours[Colour::WHITE]; } 
u64 Position::get_black_pieces() { return colours[Colour::BLACK]; } 
u64 Position::get_pieces_from_current_turn() {
    if(turn == Turn::WHITE) {
        return get_white_pieces();
    }
    else {
        return get_black_pieces();
    }
}

u64 Position::get_board() { return colours[Colour::WHITE] | colours[Colour::BLACK]; }

bool Position::get_wscr() { return castling_rights & (1 << Castling_Rights::WHITE_SHORT); }
bool Position::get_wlcr() { return castling_rights & (1 << Castling_Rights::WHITE_LONG); }
bool Position::get_bscr() { return castling_rights & (1 << Castling_Rights::BLACK_SHORT); }
bool Position::get_blcr() { return castling_rights & (1 << Castling_Rights::BLACK_LONG); }

void Position::set_turn(Turn turn) {this->turn = turn;}

// Updates pieces and colours depending on who moved what and if it took a piece and what the promotion is
void Position::set_pieces_and_colours(const Piece& moved_piece_type, const Piece& captured_piece_type, const Piece& promoted_piece_type, const Turn& turn, const u8& src_square, const u8& dest_square, bool is_en_passant) {
    // Determine who is what colour based off whose turn it is. (Turn -> colour.)
    Colour playerA_colour;
    Colour playerB_colour;
    if(turn == Turn::WHITE) {
        playerA_colour = Colour::WHITE;
        playerB_colour = Colour::BLACK;
    }
    else {
        playerA_colour = Colour::BLACK;
        playerB_colour = Colour::WHITE;
    }

    // Updating representation for piece on starting square by removing it from its starting square
    u64 moved_piece = pieces[moved_piece_type];
    moved_piece = Utils::clear_bit(moved_piece, src_square);
    pieces[moved_piece_type] = moved_piece;

    // Updating our colours
    u64 playerA_pieces = colours[playerA_colour];
    playerA_pieces = Utils::clear_bit(playerA_pieces, src_square);
    playerA_pieces |= 1ULL << dest_square;
    colours[playerA_colour] = playerA_pieces;

    // Updating representation for piece on destination square by adding it to the destination square
    // Handling promotions
    if(promoted_piece_type != Piece::INVALID) { // if there is a promotion
        pieces[promoted_piece_type] |= 1 << dest_square; // add new piece to the board
    }
    else { // if no promotion, add pawn to new square
        pieces[moved_piece_type] |= 1ULL << dest_square;
    }

    if(captured_piece_type != Piece::INVALID) { // if has been a capture, update playerB's board accordingly
        if(moved_piece_type != captured_piece_type) { // if same type, then capture has already occured for pieces board
            u64 captured_piece = pieces[captured_piece_type];
            captured_piece = Utils::clear_bit(captured_piece, dest_square);
            pieces[captured_piece_type] = captured_piece;
        }
        else if(is_en_passant) { // or if the same type has been captured, it needs to remove the piece at the en passant target.
            u64 captured_piece = pieces[captured_piece_type];
            captured_piece = Utils::clear_bit(captured_piece, en_passant_target);
            pieces[captured_piece_type] = captured_piece;
            u64 playerB_pieces = colours[playerB_colour];
            playerB_pieces = Utils::clear_bit(playerB_pieces, en_passant_target);
            colours[playerB_colour] = playerB_pieces;
            return;
        }

        u64 playerB_pieces = colours[playerB_colour];
        playerB_pieces = Utils::clear_bit(playerB_pieces, dest_square);
        colours[playerB_colour] = playerB_pieces;
    }
}

///////////////////

// Finds the best move for the current position using negamax
// Initial call for negamax for each move up to a line depth provided
Move Position::find_best_move(const Position& current_position, u8 depth) {
    // Currently do not keep track of eval after end
    // If wish to do this for programming analysis, can
    // just use prints
    int best_eval = INT_MIN;
    Move best_move = Move(0, 0, Move_Flag::NULL_FLAG);
    int eval;
    // Need to convert this to return moves instead of bitboards
    std::vector<Move> moves = generate_all_moves();
    Position new_position;
     for(Move move : moves) {
        // Reset position
        new_position = current_position;
        
        // make move
        copy_make(move, new_position); // apply move to current pos
        if(!new_position.legality_check()) { // if move isnt legal, skip eval for this move
            continue;
        }

        // depth - 1 because we have already looked at this depth
        // eg: depth 1 = our move
        // eg: depth 2 = opponents response
        // uses depth of half moves

        // Should figure out how to make use of turns
        // in this case, it should be called with the turn that isnt ours
        // for simplicity we assume we are white for now
        // calls -negamax as to flip the eval back to our perspective

        // AB Negamax:
        eval = -negamax_ab(new_position, depth - 1, INT_MIN, INT_MAX);

        if(eval > best_eval) {
            best_eval = eval;
            best_move = move;
        }
     }
     return best_move;
}

// Inspired from the chess programming wiki (standard convention)
// Search algorithm which produces the same output as minimax but is simpler to implement

// Pruning is less helpful for gambit as it aims to find risky moves, therefore pruning could prove bad
// Added pruning simply but may take it off for the above reason
// negamax and negamax_ab returns the same best move, therefore chose to implement this version.
int Position::negamax_ab(Position pos, u8 depth, int alpha, int beta) {
    if (depth == 0) { // or if checkmate?
        // returns eval of the position at a given depth
        return evaluate(pos);
    }

    std::vector<Move> moves = pos.generate_all_moves();
    int best_eval;
    for(Move move : moves) {
        pos.copy_make(move, pos);

        int eval = -negamax_ab(pos, depth - 1, -beta, -alpha);
        // do i need to undo the move? shouldnt need to if i copy positions instead
        // of editing our one position

        if(eval > best_eval) {
            best_eval = eval;
        }
        if(eval > alpha) {
            alpha = eval;

            if(beta >= eval) {
                break;
            }
        }
    }

    return best_eval;

}

//////////

int Position::evaluate(Position pos) {
    // I think chess perspective of - for black is performed in another function, so dont need to consider anything besides own side - opp side
    if(pos.get_turn() == Turn::WHITE) {
        // could make a get_opp_turn()?
        return count_material(Turn::WHITE) - count_material(Turn::BLACK);
    }
    else {
        return count_material(Turn::BLACK) - count_material(Turn::WHITE);
    } 
}

u8 Position::count_material(Turn turn) {
    u8 material = 0;
    u64 board;
    if(turn == Turn::WHITE) {
        board = get_white_pieces();
    }
    else {
        board = get_black_pieces();
    }
    Piece piece;
    while(true) {
        piece = get_piece_type_from_square(Utils::find_piece_index(board));
        switch(piece) {
            case(Piece::PAWN):
                material += Piece_Values::PAWN_VALUE;
                break;
            case(Piece::KNIGHT):
                material += Piece_Values::KNIGHT_VALUE;
                break;
            case(Piece::BISHOP):
                material += Piece_Values::BISHOP_VALUE;
                break;
            case(Piece::ROOK):
                material += Piece_Values::ROOK_VALUE;
                break;
            case(Piece::QUEEN):
                material += Piece_Values::QUEEN_VALUE;
                break;
            default: // Kings and Invalid have no value
                break;
        }
    }
    return material;
}