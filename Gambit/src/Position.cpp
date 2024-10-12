#include "Position.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>
#include <random>

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
                if (file_letter == '-') { en_passant_target = 0ULL; break; }
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

// Sets the turn to white or black
void Position::set_turn(Turn turn) {this->turn = turn;}

u64 Position::generate_pawn_attacks(int square) {
    // Generate pawn attacks
    // Pawn can move twice if it is in the 2nd rank of its colour (hasnt moved).
    // Pawn cannot move 1 or 2 if any colour piece is in the way
    // Pawn can attack diagonally if piece of opposite colour is 1 diagonal.
    // Functions to shift up, shift down, shift left and shift right.
    // Use square to get attacks.

    // Generates pawn attacks based off colour (which determines direction)
    // Each attack is added to attack using |=
    // Board is & with the compliment of get_board, as to remove any forward moves if theres pieces of any colour in the way
    // Diagonal attacks check whether they are attacking opposite colour.
    // Combine moves and attacks
    u64 pawn_pos = 1ULL << square;
    u64 moves = 0ULL;
    u64 attacks = 0ULL;
    if(turn == Turn::WHITE) {
        moves |= Utils::shift_up(pawn_pos);
        if (8 <= square && square <= 15) {moves |= Utils::shift_up(Utils::shift_up(pawn_pos));}
        moves = moves & ~get_board();
        attacks |= Utils::shift_up(Utils::shift_left(pawn_pos));
        attacks |= Utils::shift_up(Utils::shift_right(pawn_pos));
        attacks = attacks & get_black_pieces();
    }
    else {
        moves |= Utils::shift_down(pawn_pos);
        if (48 <= square && square <= 55) {moves |= Utils::shift_down(Utils::shift_down(pawn_pos));}
        moves = moves & ~get_board();
        attacks |= Utils::shift_down(Utils::shift_left(pawn_pos));
        attacks |= Utils::shift_down(Utils::shift_right(pawn_pos));
        attacks = attacks & get_white_pieces();
    }
    attacks |= moves;
    return attacks;
}

    // Common Knowledge, however was found at this link: https://github.com/TiltedDFA/TDFA/blob/TestingVsMagicBBs/src/MagicBitboards.hpp
    const int BISHOP_SHIFTS[64] = {
        58, 59, 59, 59, 59, 59, 59, 58, 
        59, 59, 59, 59, 59, 59, 59, 59, 
        59, 59, 57, 57, 57, 57, 59, 59, 
        59, 59, 57, 55, 55, 57, 59, 59, 
        59, 59, 57, 55, 55, 57, 59, 59, 
        59, 59, 57, 57, 57, 57, 59, 59, 
        59, 59, 59, 59, 59, 59, 59, 59, 
        58, 59, 59, 59, 59, 59, 59, 58
    };

   extern U64 bishopAttacksEmptyBoard[64];
    extern U64 rookAttacksEmptyBoard[64];
    extern U64 bishopAttacksTable[64][1ULL << 9ULL]; // At any given time, a bishop can be blocked by up to 9 squares (think about a rook on a1 for example)
    extern U64 rookAttacksTable[64][1ULL << 12ULL]; // At any given time, a rook can be blocked by up to 12 squares (think about a rook on a1 for example)

    

    u64 key = (blockers * magic.magic_number?) >> ROOK_SHIFTS[sq];
    rookAttacksTable[sq][key] = rookAttacksSlow(sq, blockersArrangement);

    // Number of times to right shift bits in the magic process, such that the bits are moved to the right most bits, making multiplication by a magic number for effective at producing unique values
    // For example, a rook on a1 has 12 different squares that block its movement. Therefore, 64 - 12 = 52. So the 12th bit will move to 64, with the rest behind it.
    const int ROOK_SHIFTS[64] = {
        52, 53, 53, 53, 53, 53, 53, 52, 
        53, 54, 54, 54, 54, 54, 54, 53, 
        53, 54, 54, 54, 54, 54, 54, 53, 
        53, 54, 54, 54, 54, 54, 54, 53, 
        53, 54, 54, 54, 54, 54, 54, 53, 
        53, 54, 54, 54, 54, 54, 54, 53, 
        53, 54, 54, 54, 54, 54, 54, 53, 
        52, 53, 53, 53, 53, 53, 53, 52
    };

// Inspiration drawn from: https://analog-hors.github.io/site/magic-bitboards/
Final_Magic Position::find_magic(int square) {
    MagicEntry magic;
    Final_Magic successful_magic;
    const int index_bits = ROOK_SHIFTS[square];
    bb_vector magic_table;

    // Create a random device
    std::random_device rd;

    // Initialize a Mersenne Twister pseudo-random number generator
    std::mt19937_64 gen(rd());

    // Define the range of random numbers (inclusive)
    std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
    magic.mask = get_relevant_blocker_squares(square); // Bitboard of spaces where blockers would be an issue
    magic.index_bits = ROOK_SHIFTS[square];
    while(true) {
        // Magics require a low number of active bits, so we AND
        // by two more random values to cut down on the bits set.
        magic.magic_number = dis(gen) & dis(gen) & dis(gen);
        magic_table = create_magic_table(magic, square);
        successful_magic.magic = magic;
        if (magic_table.size() == ((std::size_t)(1 << index_bits))) { // if full table (meaning no collisions or missed computations)
            successful_magic.table = magic_table;
            return successful_magic;
        }
    }
}

 size_t get_magic_index(MagicEntry magic, u64 blockers) {
    blockers &= magic.mask; // ands this blocker combination with the block attack mask
    // Perform the magic multiplication (wrapping multiply equivalent)
    uint64_t hash = blockers * magic.magic_number;

    // Calculate the index by shifting the hash
    size_t index = static_cast<size_t>(hash >> (64 - magic.index_bits));

    return index;
 }

// Attempts to make the table used for the precomputations of magic attacks
bb_vector Position::create_magic_table(MagicEntry magic, int square) {
    bb_vector table(1 << magic.index_bits, 0ULL);
    std::cout << table.size() << "\n";
    u64 moves;
    for(u64 a : table) {
        //Utils::Print_BB
        std::cout << a << "\n";
    }
    for(u64 blockers : get_blocker_combinations(magic.mask)) {
        std::cout << "1\n";
        moves = pseudo_legalise_rook_attacks_slow(square, blockers);
        std::cout << "2\n";
        // Calculate the table index based on the magic number
        size_t index = get_magic_index(magic, blockers);
        std::cout << "3\n";
        if(table[index] == 0ULL) { // if empty
        std::cout << "4\n";
            table[index] = moves;
        }
        else if (table[index] != moves) { // if collision
            return bb_vector(0);
        }
    }
    return table;
}

// Gets the piece type of a piece on a given square
Piece Position::get_piece_type(int square) {
    u64 piece_pos = 1ULL << square;
    if(get_pawns() & piece_pos) {return Piece::PAWN;}
    else if (get_knights() & piece_pos) {return Piece::KNIGHT;}
    else if (get_bishops() & piece_pos) {return Piece::BISHOP;}
    else if (get_rooks() & piece_pos) {return Piece::ROOK;}
    else if (get_queens() & piece_pos) {return Piece::QUEEN;}
    else if (get_kings() & piece_pos) {return Piece::KING;}
    else {return Piece::INVALID;} // If no piece on square
}

// Gets squares where pieces are that block the sliding piece from moving past it
u64 Position::get_blockers(int square) {
    return Utils::ROOK_ATTACKS_NO_EDGES[square] & get_board();
}

// Gets squares where if a piece were on it, it would block the sliding piece from moving past it
u64 Position::get_relevant_blocker_squares(int square) {
    return Utils::ROOK_ATTACKS_NO_EDGES[square];
}

bb_vector Position::get_blocker_combinations(u64 blockers) {
    bb_vector blocker_combinations;
    // Start from the full mask and generate all subsets
    u64 subset = blockers;
    do {
        blocker_combinations.push_back(subset);
        subset = (subset - 1) & blockers;  // Move to the next subset
    } while (subset != blockers);  // Stop when we cycle back to the full mask

    return blocker_combinations;
}

u64 Position::pseudo_legalise_rook_attacks_slow(int square, u64 blockers) {
    // Shift in all directions until meet certain criteria
    u64 attacks_after_blockers = 0;
    int temp_square = 0; 
    u64 temp_bb;

    for(temp_square = square + 1; (temp_square) % 8 != 0; temp_square++) { // Checking Right
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }

    for(temp_square = square - 1; (temp_square + 1) % 8 != 0 && temp_square >= 0; temp_square--) { // Checking Left
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }   

    for(temp_square = square; temp_square < 64; temp_square += 8) { // Checking Up
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }

    for(temp_square = square; temp_square > 0; temp_square -= 8) { // Checking Down
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }
    attacks_after_blockers ^= 1ULL << square; // Remove current square
    return attacks_after_blockers; // Actual location of blocking pieces
}

//////////////////////////////////

// bb_vector Position::pseudo_legalise_rook_attacks(int square, u64 attacks) {
// // Uninterrupted attack mask (standard lookup from the ROOK_ATTACKS etc)
// // Relevant blocking squares mask (if another piece would on this, itd block the rook from getting past)
// // Actual squares of blocking pieces (occupancy bitboard) by & with board mask
// // Multiply by magic number to access lookup table
// // Use this index to get the attack mask for the rook on a1, considering the occupancy bitboard.

// // 1. attacks
// // 2. 
//     // Shift in all directions until meet certain criteria
//     u64 blocking_squares = get_blockers(square);
//     bb_vector new_attacks = get_attacks_after_blockers(square, attacks, get_blocker_combinations(blocking_squares));
//     // pass in vector

//     find_magic(new_attacks, square); //////////////////////////////////////////////////////////////////////////////////////
//     return bb_vector(0);
// }

bb_vector Position::extract_piece_moves(u64 attacks) {
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

bb_vector Position::generate_piece_moves(int square, Piece type) { // will be used when searching for both sides, so will use Turn.
    u64 attacks = 0ULL;
    switch(type) {
        case Piece::PAWN:
            attacks = generate_pawn_attacks(square);
            return extract_piece_moves(attacks);
        case Piece::KNIGHT:
            attacks = Utils::KNIGHT_ATTACKS[square];
            break;
        case Piece::BISHOP:
            attacks = Utils::BISHOP_ATTACKS[square];
            break;
        case Piece::ROOK:
            attacks = Utils::ROOK_ATTACKS[square];
            //attacks = pseudo_legalise_rook_attacks(square, attacks);
            break;
        case Piece::QUEEN:
            attacks = Utils::QUEEN_ATTACKS[square];
            break;
        case Piece::KING:
            attacks = Utils::KING_ATTACKS[square];
            break;
        case Piece::INVALID:
            throw std::invalid_argument("Recieved invalid piece type");
            break;
    }
    if(turn == Turn::WHITE) {
        attacks = attacks & ~get_white_pieces();
    }
    else {
        attacks = attacks & ~get_black_pieces();
    }
    return extract_piece_moves(attacks);
}

std::array<u64, 6> Position::get_pieces() { return pieces; }
std::array<u64, 2> Position::get_colours() { return colours; }
u64 Position::get_en_passant_target() { return en_passant_target; }
int Position::get_half_move_clock() { return half_move_clock; }
int Position::get_full_move_counter() { return full_move_counter; }
u8 Position::get_castling_rights() { return castling_rights; }
Turn Position::get_turn() { return turn; }

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

u64 Position::get_board() { return colours[Colour::WHITE] | colours[Colour::BLACK]; }

bool Position::get_wscr() { return castling_rights & (1 << Castling_Rights::WHITE_SHORT); }
bool Position::get_wlcr() { return castling_rights & (1 << Castling_Rights::WHITE_LONG); }
bool Position::get_bscr() { return castling_rights & (1 << Castling_Rights::BLACK_SHORT); }
bool Position::get_blcr() { return castling_rights & (1 << Castling_Rights::BLACK_LONG); }