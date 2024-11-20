#include "Position.hpp"
#include "Magics.hpp"

#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>
#include <cassert>
#include <random>


Position::Position(const std::string& fen):
    pieces(),
        colours(),
            en_passant_target(), // Initializes to the default value. Can alternatively put whatever value here i want.
            // 64 if not applicable. This is the square of the pawn that just moved twice. Not the dest square of en passant.
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
            u64 square = 1ULL << (rank * 8 + file);
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
                if (file_letter == '-') { en_passant_target = Utils::NULL_EN_PASSANT; break; }
                else if (file_letter == 'a') { file_number = 0; }
                else if (file_letter == 'b') { file_number = 1; }
                else if (file_letter == 'c') { file_number = 2; }
                else if (file_letter == 'd') { file_number = 3; }
                else if (file_letter == 'e') { file_number = 4; }
                else if (file_letter == 'f') { file_number = 5; }
                else if (file_letter == 'g') { file_number = 6; }
                else { file_number = 7; } // if == 'h'
                en_passant_target = (((word[1] - '0' - 1) * 8) + file_number);
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
    u64 board = get_board();
    if (get_turn() == Turn::WHITE) {
        attacks = Utils::WHITE_PAWN_ATTACKS[square];
        attacks &= ~get_white_pieces();
        if (square + 8 < 64 && Utils::piece_is_at_square(board, square + 8)) { // if something directly in front and is on the board, can't move forward
            attacks &= ~(1ULL << (square + 8));
        }
        if (square >= 8 && square <= 15 && 
            (Utils::piece_is_at_square(board, square + 16) || Utils::piece_is_at_square(board, square + 8))) {
            attacks &= ~(1ULL << (square + 16));
        }
        if ((square + 7 < 64) && ((square + 7 + 1) % 8 != 0) && !(Utils::piece_is_at_square(get_black_pieces(), square + 7)) && 
            ((square % 8) != 0)) { // left diagonal capture check, ensuring within board boundaries
            attacks &= ~(1ULL << (square + 7));
        }
        if ((square + 9 < 64) && ((square + 9) % 8 != 0) && !(Utils::piece_is_at_square(get_black_pieces(), square + 9)) && 
            ((square % 8) != 7)) { // right diagonal capture check, ensuring within board boundaries
            attacks &= ~(1ULL << (square + 9));
        }
        if (en_passant_target != 64) { // En passant capture logic
            if (((square + 7 == en_passant_target) && (((square + 7 + 1) % 8 != 0))) || ((square + 9 == en_passant_target) && (((square + 9) % 8 != 0)))) {
                attacks |= 1ULL << en_passant_target;
            }
        }
    } else {
        attacks = Utils::BLACK_PAWN_ATTACKS[square];
        attacks &= ~get_black_pieces();
        if (square - 8 >= 0 && Utils::piece_is_at_square(board, square - 8)) {
            attacks &= ~(1ULL << (square - 8));
        }
        if (square >= 48 && square <= 55 && 
            (Utils::piece_is_at_square(board, square - 16) || Utils::piece_is_at_square(board, square - 8))) {
            attacks &= ~(1ULL << (square - 16));
        }
        if ((square - 7 >= 0) && ((square - 7) % 8 != 0) && !(Utils::piece_is_at_square(get_white_pieces(), square - 7)) && 
            ((square % 8) != 7)) { // right diagonal capture check for black pawns
            attacks &= ~(1ULL << (square - 7));
        }
        if ((square - 9 >= 0) && ((square - 9 + 1) % 8 != 0) && !(Utils::piece_is_at_square(get_white_pieces(), square - 9)) && 
            ((square % 8) != 0)) { // left diagonal capture check for black pawns
            attacks &= ~(1ULL << (square - 9));
        }
        if (en_passant_target != 64) {
            if (((square - 7 == en_passant_target) && ((square - 7) % 8 != 0)) || ((square - 9 == en_passant_target) && ((square - 9 + 1) % 8 != 0))) {
                attacks |= 1ULL << en_passant_target;
            }
        }
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
            if(get_wscr()) {
                if(get_white_rooks() & (1ULL << 7)) {
                    if(!(Utils::piece_is_at_square(board, 5) || Utils::piece_is_at_square(board, 6))) {
                        moves |= 1ULL << 6; // add short castling
                    }
                }
            }
            if(get_wlcr()) {
                if(get_white_rooks() & (1ULL)) {
                    if(!(Utils::piece_is_at_square(board, 1) || Utils::piece_is_at_square(board, 2) || Utils::piece_is_at_square(board, 3))) {
                        moves |= 1ULL << 2; // long
                    }
                }
            }
        }
    }
    else {
        if(square == 60) {
            u64 board = get_board();
            if(get_bscr()) { // if allowed to castle
                if(get_black_rooks() & (1ULL << 63)) {
                    if(!(Utils::piece_is_at_square(board, 61) || Utils::piece_is_at_square(board, 62))) { // if clear space between
                        moves |= 1ULL << 62; // add short castling
                    }
                }
            }
            if(get_blcr()) {
                if(get_black_rooks() & (1ULL << 56)) {
                    if(!(Utils::piece_is_at_square(board, 59) || Utils::piece_is_at_square(board, 58) || Utils::piece_is_at_square(board, 57))) {
                        moves |= 1ULL << 58; // long
                    }
                }
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
    for(u8 square = 0; square < 64; square++) { // can change this so it only iterates through pieces mask but can do some other time
        if((1ULL << square) & pieces) { // if the person whos turn it is has a piece on this square, gen its moves
            Piece piece_type = get_piece_type_from_square(square);
            square_moves = generate_piece_moves(piece_type, square);
            for(Move move : square_moves) {
                moves.push_back(move);
                // if pawn destination square is unoccupied, en passant flag
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
    return attacks & ~get_pieces_from_current_turn();
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
    return attacks & ~get_pieces_from_current_turn();
}

u64 Position::get_queen_moves(u8 square) {
    // Returns the attack mask of a queen on a given square
    // Does this by glueing a rook and bishop on the same square together
    u64 attacks = get_rook_moves(square) | get_bishop_moves(square);
    return attacks & ~get_pieces_from_current_turn();
}

u64 Position::generate_piece_attacks(Piece type, u8 square) { // returns u64 without move conversion
    u64 attacks = 0ULL;
    switch(type) {
        case Piece::PAWN:
            attacks = get_pawn_attacks(square);
            return attacks; // skip check for piece colours as already handled.
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
    // Ensuring it isnt attacking its own piece

    return attacks & ~get_pieces_from_current_turn();
}

std::vector<Move> Position::generate_piece_moves(Piece type, u8 square) { // will be used when searching for both sides, so will use Turn.
    u64 attacks = 0ULL;
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
            attacks = get_king_moves(square);
            break;
        case Piece::INVALID:
            attacks = 0;
            break;
    }
    // Ensuring it isnt attacking its own piece

    attacks &= ~get_pieces_from_current_turn();

    return bb_to_move_list(type, square, attacks);
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
        move_list.push_back(Utils::encode_move(type, src_square, dest_square, en_passant_target));
    }
    return move_list;
}

// Checks whether after applying a pseudo move, if the position is legal.
bool Position::legality_check(Move& move)
{
    // Finding King Square and opponents pieces:
    u64 king_square_u64;
    u64 our_pieces;
    u64 opponent_pieces;
    if(turn == Turn::BLACK) { // is flipped because we flip turn after make move
        our_pieces = get_white_pieces();
        opponent_pieces = get_black_pieces();
        king_square_u64 = get_white_king();
    }
    else {
        our_pieces = get_black_pieces();
        opponent_pieces = get_white_pieces();
        king_square_u64 = get_black_king();
    }

    if(king_square_u64 == 0ULL) { // if king has been taken. Should not happen but will be helpful for strange FENs
        return false;
    }

    u8 king_square_index = Utils::find_piece_index(king_square_u64);

    // Finding if king is in check
    // Finding squares where if a piece is on, would see the king.
    // OR queen attacks from the square with knight attacks
    // generate attacks for opponents pieces on these squares
    u64 king_target_mask = Utils::QUEEN_ATTACKS[king_square_index];
    king_target_mask |= Utils::KNIGHT_ATTACKS[king_square_index]; // squares where if opponents piece is on could attack king
    u64 opponent_relevant_pieces = king_target_mask & opponent_pieces; // squares with an opponents piece on
    
    // Check if opponents relevant pieces can actually attack the king
    // Is there a faster way than generating them?
    // Generate attacks for pieces on these squares
    // if return val & king_square_u64 then return false

    u8 index;
    u64 attacks;
    Piece type;
    while(opponent_relevant_pieces) {
        index = Utils::find_piece_index(opponent_relevant_pieces);
        type = get_piece_type_from_square(index);
        attacks = generate_piece_attacks(type, index);
        if(attacks & king_square_u64) { // if there is a piece hitting the king
            return false;
        }
        opponent_relevant_pieces = Utils::clear_bit(opponent_relevant_pieces, index);
    }


    if(move.get_flag() == Move_Flag::CASTLING_FLAG) { // Already checks when generating whether it has the rights to do so
        // Already checks if king moved into check
        // Now need to check whether the squares the king traverses are in check, or whether the king was in check
        u8 relevant_square_1;
        u8 relevant_square_2;
        u64 relevant_squares = 0ULL;
        if(move.get_dest_square() == 2) {
            relevant_square_1 = 2;
            relevant_square_2 = 3;
        }
        else if(move.get_dest_square() == 6) {
            relevant_square_1 = 5;
            relevant_square_2 = 6;
        }
        else if(move.get_dest_square() == 58) {
            relevant_square_1 = 58;
            relevant_square_2 = 59;
        }
        else if(move.get_dest_square() == 62) {
            relevant_square_1 = 61;
            relevant_square_2 = 62;
        }
        else {
            std::cout << "\nExpected castling but king did not move to the expected square\n";
            assert(0);
        }

        u8 previous_king_square = move.get_src_square();
        u64 target_mask = Utils::QUEEN_ATTACKS[relevant_square_1];
        target_mask |= Utils::QUEEN_ATTACKS[relevant_square_2];
        target_mask |= Utils::QUEEN_ATTACKS[previous_king_square];
        target_mask |= Utils::KNIGHT_ATTACKS[relevant_square_1]; // squares where if opponents piece is on could attack the square a king has to move through
        target_mask |= Utils::KNIGHT_ATTACKS[relevant_square_2];
        target_mask |= Utils::KNIGHT_ATTACKS[previous_king_square];
        opponent_relevant_pieces = target_mask & opponent_pieces; // squares with an opponents piece on

        while(opponent_relevant_pieces) {
            index = Utils::find_piece_index(opponent_relevant_pieces);
            type = get_piece_type_from_square(index);
            attacks = generate_piece_attacks(type, index);
            if(type == Piece::PAWN) {
                if(turn == Turn::BLACK) {
                    if((index - 7 == move.get_src_square()) || (index - 9 == move.get_src_square())) { // if pawn could see king before move
                        attacks |= (1ULL << move.get_src_square());
                    }
                }
                else {
                    if((index + 7 == move.get_src_square()) || (index + 9 == move.get_src_square())) {
                        attacks |= (1ULL << move.get_src_square());
                    }
                }
            }
            if((attacks & (1ULL << relevant_square_1)) || (attacks & (1ULL << relevant_square_2)) || (attacks & (1ULL << previous_king_square))) {
                return false;
            }
            opponent_relevant_pieces = Utils::clear_bit(opponent_relevant_pieces, index);
        }
    }

    return true;
}

// Applies changes to a copy of the Position object that calls it. Returns said copy.
void Position::make_move(Move& move) // simpler than make and unmake but slightly slower.
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
        case ROOK_FLAG: // removing castling rights
            if(castling_rights == 0) { // if already cant castle, ignore removing castling
                break;
            }
            if(src_square == 0) { // no need to check colour as this implies if either rook got here that they cant castle
                remove_wlcr();
            }
            else if(src_square == 7) {
                remove_wscr();
            }
            else if(src_square == 56) {
                remove_blcr();
            }
            else if(src_square == 63) {
                remove_bscr();
            }
            break;
        case KING_FLAG:
            if(turn == Turn::WHITE) {
                remove_wlcr();
                remove_wscr();
            }
            else {
                remove_blcr();
                remove_bscr();
            }
            break;

        case PAWN_TWO_FORWARD_FLAG:
            if(turn == Turn::WHITE) {
                en_passant_target = dest_square - 8;
            }
            else {
                en_passant_target = dest_square + 8;
            }

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

            //ref//
        case CASTLING_FLAG:
            // remove corresponding castling rights and play rook portion. After this it will move king as if normal move.
            if(turn == Turn::WHITE) {
                if(dest_square == 6) { // has short castled
                    set_pieces_and_colours(Piece::ROOK, Piece::INVALID, Piece::INVALID, turn, 7, 5, false);
                }
                else { // has long castled
                    set_pieces_and_colours(Piece::ROOK, Piece::INVALID, Piece::INVALID, turn, 0, 3, false);
                }
                remove_wscr();
                remove_wlcr();
            }
            else { // if black
                if(dest_square == 62) { // has short castled
                    set_pieces_and_colours(Piece::ROOK, Piece::INVALID, Piece::INVALID, turn, 63, 61, false);
                }
                else { // has long castled
                    set_pieces_and_colours(Piece::ROOK, Piece::INVALID, Piece::INVALID, turn, 56, 59, false);
                }
                remove_bscr();
                remove_blcr();
            }
            break;
    }
    // Update pieces and colours.
    // Does this if it isnt an en passant. Other specials make use of this, for example castling plays this as a rook move previously
    if (flag != EN_PASSANT_FLAG) {
        set_pieces_and_colours(piece_type, captured_piece_type, promoted_piece_type, turn, src_square, dest_square, false);
    }

    if ((en_passant_target != Utils::NULL_EN_PASSANT) && (flag != PAWN_TWO_FORWARD_FLAG)) {en_passant_target = Utils::NULL_EN_PASSANT;} // reset en passant if double push hasnt been played
    turn = get_opp_turn();
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

void Position::remove_wscr() { castling_rights = castling_rights & ~(1 << Castling_Rights::WHITE_SHORT); }
void Position::remove_wlcr() { castling_rights = castling_rights & ~(1 << Castling_Rights::WHITE_LONG); }
void Position::remove_bscr() { castling_rights = castling_rights & ~(1 << Castling_Rights::BLACK_SHORT); }
void Position::remove_blcr() { castling_rights = castling_rights & ~(1 << Castling_Rights::BLACK_LONG); }

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
        pieces[promoted_piece_type] |= 1ULL << dest_square; // add new piece to the board
    }
    else { // if no promotion, add piece to new square
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
            if(playerB_colour == Colour::WHITE) {
                captured_piece = Utils::clear_bit(captured_piece, en_passant_target + 8); // we have captured whites piece with en passant
                    pieces[captured_piece_type] = captured_piece;
                u64 playerB_pieces = colours[playerB_colour];
                playerB_pieces = Utils::clear_bit(playerB_pieces, en_passant_target + 8);
                colours[playerB_colour] = playerB_pieces;
            }
            else {
                captured_piece = Utils::clear_bit(captured_piece, en_passant_target - 8);
                pieces[captured_piece_type] = captured_piece;
                u64 playerB_pieces = colours[playerB_colour];
                playerB_pieces = Utils::clear_bit(playerB_pieces, en_passant_target - 8);
                colours[playerB_colour] = playerB_pieces;
            } // ref //
            return;
        }

        u64 playerB_pieces = colours[playerB_colour];
        playerB_pieces = Utils::clear_bit(playerB_pieces, dest_square);
        colours[playerB_colour] = playerB_pieces;
    }
}

Move Position::find_random_move() {
    std::vector<Move> moves = generate_all_moves();
     std::random_device rd;
    // Initialize a Mersenne Twister pseudo-random number generator
    std::mt19937_64 gen(rd());
    // Define the range of random numbers (inclusive)
    std::uniform_int_distribution<uint64_t> dis(0, moves.size());
    while(true) {
        Position new_position = *this; // reset position after every exploration
        // make move
        Move move = moves[dis(gen)];
        new_position.make_move(move); // apply move to current pos

        if(new_position.legality_check(move)) { // if move isnt legal, skip eval for this move
                return move;
            }
    }
}

// Finds the best move for the current position using negamax
// Initial call for negamax for each move up to a line depth provided
Move Position::find_best_move(Timer& timer) {
    // Currently do not keep track of eval after end
    // If wish to do this for programming analysis, can
    // just use prints

    int best_eval = INT_MIN;
    Move best_move = Move(0, 0, Move_Flag::NULL_FLAG);
    int eval;
    for(u8 depth = 1; !timer.is_out_of_time(); depth++) { // increments depth of search until runs out of time
        std::vector<Move> moves = generate_all_moves();
        for(Move move : moves) {
            // Reset position
            Position new_position = *this; // reset position after every exploration
            // make move
            new_position.make_move(move); // apply move to current pos

            if(!new_position.legality_check(move)) { // if move isnt legal, skip eval for this move
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
            eval = -new_position.negamax_ab(depth - 1, INT_MIN, INT_MAX);
            if(eval > best_eval) {
                best_eval = eval;
                best_move = move;
            }
        }
    }
     return best_move;
}

// Inspired from the chess programming wiki (standard convention)
// Search algorithm which produces the same output as minimax but is simpler to implement

// Pruning is less helpful for gambit as it aims to find risky moves, therefore pruning could prove bad
// Added pruning simply but may take it off for the above reason
// negamax and negamax_ab returns the same best move, therefore chose to implement this version.
int Position::negamax_ab(u8 depth, int alpha, int beta) {
    Position new_position = *this;
    
    if (depth == 0) { // or if checkmate?
        // returns eval of the position at a given depth
        //std::cout << evaluate(current_position);
        return evaluate(new_position);
    }

    int best_eval;
    for(Move move : generate_all_moves()) {
        // Reset position
        new_position = *this; // reset position after every exploration
        
        // make move
        new_position.make_move(move); // apply move to current pos
        
        if(!new_position.legality_check(move)) { // if move isnt legal, skip eval for this move
            continue;
        }

        int eval = -new_position.negamax_ab(depth - 1, -beta, -alpha);
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

        return pos.count_material(Turn::WHITE) - pos.count_material(Turn::BLACK);
    }
    else {
        return pos.count_material(Turn::BLACK) - pos.count_material(Turn::WHITE);
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
    int square = 0;
    while(true) {
        // find ls1b square
        square = Utils::find_piece_index(board);
        if(square == 64) { // no more pieces
            break;
        }
        piece = get_piece_type_from_square(square);
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
        board = Utils::clear_bit(board, square);
    }
    return material;
}

u64 Position::split_perft(int current_depth, const int& desired_depth, const bool& output_split) // desired depth being the initial depth input
{
    if (current_depth == 0) {
        return 1ULL;
    }
    
    u64 sum = 0;
    u64 nodes = 0;
    std::vector<Move> moves = generate_all_moves();
    for(Move& move : moves) {
        // Reset position
        // Create a copy of the current starting position at each depth
        Position new_position = *this;
        new_position.make_move(move); // apply move to current pos

        if(!new_position.legality_check(move)) { // if move isnt legal, don't count
            continue;
        }

        nodes = new_position.split_perft(current_depth - 1, desired_depth, output_split);
        sum += nodes;

        if(output_split) {
            if(current_depth == desired_depth) { // if finished recursion
                std::cout << Utils::move_to_board_notation(move) << ": " << nodes << std::endl;
            }
        }
    }
    return sum;
}