#include "Position.hpp"

#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>
#include <cassert>
#include <bit>
#include <bitset>
#include <random>


Position::Position(const std::string& fen):
    pieces(),
    colours(),
    en_passant_target(), // Initializes to the default value. Can alternatively put whatever value here i want.
    // 64 if not applicable. This is the square of the pawn that just moved twice. Not the dest square of en passant.
    half_move_clock(),
    full_move_counter(),
    castling_rights(),
    turn(Turn::WHITE),
    zobrist_key() {

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
                //using namespace Types;
                // Bit boards are all 0s, by using or, the 1 is added in.
                // Need to use or instead of equals as after the first pawn is detected,
                // square wont remember where the 1 was originally for the first pawn
                case 'P': pieces[Piece::PAWN] |= square; colours[Turn::WHITE] |= square; break;
                case 'N': pieces[Piece::KNIGHT] |= square; colours[Turn::WHITE] |= square; break;
                case 'B': pieces[Piece::BISHOP] |= square; colours[Turn::WHITE] |= square; break;
                case 'R': pieces[Piece::ROOK] |= square; colours[Turn::WHITE] |= square; break;
                case 'Q': pieces[Piece::QUEEN] |= square; colours[Turn::WHITE] |= square; break;
                case 'K': pieces[Piece::KING] |= square; colours[Turn::WHITE] |= square; break;
                case 'p': pieces[Piece::PAWN] |= square; colours[Turn::BLACK] |= square; break;
                case 'n': pieces[Piece::KNIGHT] |= square; colours[Turn::BLACK] |= square; break;
                case 'b': pieces[Piece::BISHOP] |= square; colours[Turn::BLACK] |= square; break;
                case 'r': pieces[Piece::ROOK] |= square; colours[Turn::BLACK] |= square; break;
                case 'q': pieces[Piece::QUEEN] |= square; colours[Turn::BLACK] |= square; break;
                case 'k': pieces[Piece::KING] |= square; colours[Turn::BLACK] |= square; break;
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
    compute_zobrist_key(); // updates zobrist key
}

void Position::compute_zobrist_key() {
    // a XOR b = c
    // c XOR a = b
    // If turn is white, then zobrist key will not be XORd.
    // This state represents white.
    u64 temp_board = get_board();
    for(u8 sq = Utils::find_ls1b_index(temp_board); temp_board; temp_board = Utils::clear_bit(temp_board, sq), sq = Utils::find_ls1b_index(temp_board))
    {
        zobrist_key ^= (Zobrist::get_piece(get_piece_type_from_square(sq) + (6 * get_colour_value_from_square(sq)), sq));
    }
    if(en_passant_target != Utils::NULL_EN_PASSANT) {
        zobrist_key ^= Zobrist::get_en_passant(en_passant_target);
    }

    assert(!(castling_rights & 0xf0)); // if castling rights is valid (not more than 16 as this is an invalid castling rights)
    //if(static_cast<int>(castling_rights) != 0) {
     zobrist_key ^= Zobrist::get_castling_rights(castling_rights);
    //}
    zobrist_key ^= (Zobrist::get_side_to_move() * static_cast<int>(turn));
}

void Position::recompute_zobrist_key() {
    zobrist_key = 0ULL;
    compute_zobrist_key();
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

u64 Position::get_pawn_attacks(u8 square) const {
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


u64 Position::get_knight_moves(u8 square) const {
    return Utils::KNIGHT_ATTACKS[square];
}

u64 Position::get_king_moves(u8 square) const {
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
Piece Position::get_piece_type_from_square(u8 square) const {
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
 std::vector<u64> Position::extract_piece_moves(u64 attacks) const {
    std::vector<u64> attacks_vector;  // Use a vector for dynamic storage
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

std::vector<Move> Position::generate_all_moves(bool is_qsearch) const // uses the board state to generate all moves for a given colour 
{
    std::vector<Move> moves;
    u64 pieces = get_pieces_from_current_turn();
    std::vector<Move> square_moves;
    std::vector<Move> empty_vector; // no moves for this square as the player whos turn it is does not have a piece on it
    for(u8 square = 0; square < 64; square++) { // can change this so it only iterates through pieces mask but can do some other time
        if((1ULL << square) & pieces) { // if the person whos turn it is has a piece on this square, gen its moves
            Piece piece_type = get_piece_type_from_square(square);
            square_moves = generate_piece_moves(piece_type, square, is_qsearch);
            for(Move move : square_moves) {
                moves.push_back(move);
                // if pawn destination square is unoccupied, en passant flag
            }
        }
    }
    return moves;
}

u64 Position::get_rook_moves(u8 square) const {
    // Returns the attack mask of a rook on a given square
    std::vector<u64> attacks_on_square = rook_magics_table[square];
    MagicEntry magic;
    magic.magic_number = rook_magics[square];
    magic.mask = Magics::get_relevant_blocker_squares(Piece::ROOK, square);
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);
    u64 blockers = Magics::get_blockers(Piece::ROOK, square, get_board());
    u64 attacks = attacks_on_square[Magics::get_magic_index(magic, blockers)];
    return attacks & ~get_pieces_from_current_turn();
}

u64 Position::get_bishop_moves(u8 square) const {
    // Returns the long table of moves for a rook on that square. To choose which ones apply, you should return 
    std::vector<u64> attacks_on_square = bishop_magics_table[square];
    MagicEntry magic;
    magic.magic_number = bishop_magics[square];
    magic.mask = Magics::get_relevant_blocker_squares(Piece::BISHOP, square);
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);
    u64 blockers = Magics::get_blockers(Piece::BISHOP, square, get_board());
    u64 attacks = attacks_on_square[Magics::get_magic_index(magic, blockers)];
    return attacks & ~get_pieces_from_current_turn();
}

u64 Position::get_queen_moves(u8 square) const {
    // Returns the attack mask of a queen on a given square
    // Does this by glueing a rook and bishop on the same square together
    u64 attacks = get_rook_moves(square) | get_bishop_moves(square);
    return attacks & ~get_pieces_from_current_turn();
}

u64 Position::generate_piece_attacks(Piece type, u8 square) const { // returns u64 without move conversion
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

std::vector<Move> Position::generate_piece_moves(Piece type, u8 square, bool is_qsearch) const { // will be used when searching for both sides, so will use Turn.
    u64 attacks = 0ULL;
    switch(type) {
        case Piece::PAWN:
            attacks = get_pawn_attacks(square);
            break;
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

    //if(qsearch) {attacks &= get_opponent_pieces()}
    if(is_qsearch)
    {
        // pos.get_opponent_pieces() // where gets the pieces of the opposite turn
        // // think about ways of indexing rather than using if ... else ...

        if(turn == Turn::WHITE)
        {
            attacks &= get_black_pieces(); // ensure only captures considered
        }
        else
        {
            attacks &= get_white_pieces();
        }
    }

    return bb_to_move_list(type, square, attacks);
}

std::vector<Move> Position::bb_to_move_list(Piece type, u8 src_square, u64 attacks) const {
    std::vector<Move> move_list;
    u8 dest_square;
    std::vector<u64> bb_list = extract_piece_moves(attacks);
    for(u64 attack : bb_list) {
        dest_square = Utils::find_ls1b_index(attack);
        if(type == Piece::PAWN) {
            if((dest_square >= 56 && dest_square < 64) || (dest_square < 8)) { // promotion has occured
            // We dont care about colour as pawns cant get to their own end rank
                // if promotion, add promotion moves and continue as to not add a pawn move
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

bool Position::in_check()
{
    u64 king_square_u64;
    u64 opponent_pieces;
    if(turn == Turn::WHITE)
    { // is flipped because we flip turn after make move, so we check if we are in check
        opponent_pieces = get_black_pieces();
        king_square_u64 = get_white_king();
    }
    else
    {
        opponent_pieces = get_white_pieces();
        king_square_u64 = get_black_king();
    }
    if(king_square_u64 == 0ULL)
    { // if king has been taken. Should not happen but will be helpful for strange FENs
        std::cerr << "King does not exist" << std::endl;
        std::abort();
    }

    u8 king_square_index = Utils::find_ls1b_index(king_square_u64);

    // Finding if king is in check
    // Finding squares where if a piece is on, would see the king.
    // OR queen attacks from the square with knight attacks
    // generate attacks for opponents pieces on these squares


    turn = get_opp_turn(); // turn is never flipped by make move. Need to restructure my functions but this is a temp fix

    
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
    while(opponent_relevant_pieces)
    {
        index = Utils::find_ls1b_index(opponent_relevant_pieces);
        type = get_piece_type_from_square(index);
        attacks = generate_piece_attacks(type, index); // if white king is attacked by white queen, wont return that because i remove self attacks in move gen
        if(attacks & king_square_u64)
        { // if there is a piece hitting the king
            turn = get_opp_turn();
            return true;
        }
        opponent_relevant_pieces = Utils::clear_bit(opponent_relevant_pieces, index);
    }
    turn = get_opp_turn();
    return false;
}
// Checks whether after applying a pseudo move, if the position is legal.
bool Position::is_legal(Move& move) const
{
    // Finding King Square and opponents pieces:
    u64 king_square_u64;
    u64 opponent_pieces;
    if(turn == Turn::BLACK) { // is flipped because we flip turn after make move
        opponent_pieces = get_black_pieces();
        king_square_u64 = get_white_king();
    }
    else {
        opponent_pieces = get_white_pieces();
        king_square_u64 = get_black_king();
    }

    if(king_square_u64 == 0ULL) { // if king has been taken. Should not happen but will be helpful for strange FENs
        return false;
    }

    u8 king_square_index = Utils::find_ls1b_index(king_square_u64);

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
        index = Utils::find_ls1b_index(opponent_relevant_pieces);
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
            std::cerr << "\nExpected castling but king did not move to the expected square";
            std::exit(-1);
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
            index = Utils::find_ls1b_index(opponent_relevant_pieces);
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

/**
 * @brief Updates internal board state of pieces and colours, handles all moves besides en passant and castling.
 * 
 * @param src_square_type the piece type that is moving
 * @param dest_square_type the piece type that should be on the dest square after move (varies on promotion)
 * @param captured_piece_type the piece type of a captured piece (if applicable)
 * @param src_square 
 * @param dest_square 
 */
void Position::handle_move(const Piece& src_square_type, const Piece& dest_square_type, const Piece& captured_piece_type, const u8& src_square, const u8& dest_square)
{   // Update pieces:
    pieces[src_square_type] = Utils::clear_bit(pieces[src_square_type], src_square);
    if(captured_piece_type != Piece::INVALID) { pieces[captured_piece_type] = Utils::clear_bit(pieces[captured_piece_type], dest_square); }
    pieces[dest_square_type] |= 1ULL << dest_square;

    // Update colours:
    colours[turn] = Utils::clear_bit(colours[turn], src_square);
    if(captured_piece_type != Piece::INVALID) { colours[!turn] = Utils::clear_bit(colours[!turn], dest_square); }
    colours[turn] |= 1ULL << dest_square;

    // Zobrist: Update src, capture and desc square
    zobrist_key ^= Zobrist::get_piece(src_square_type + (6 * static_cast<int>(turn)), src_square); // remove from src square
    if(captured_piece_type != Piece::INVALID) {
        zobrist_key ^= Zobrist::get_piece(captured_piece_type + (6 * static_cast<int>(!turn)), dest_square); // remove opponents piece from the dest square
    }
    zobrist_key ^= Zobrist::get_piece(dest_square_type + (6 * static_cast<int>(turn)), dest_square); // add to dest square
}

/**
 * @brief Updated internal board state of pieces and colours for en passant
 * 
 * @param src_square 
 * @param dest_square 
 */
void Position::handle_en_passant(const u8& src_square, const u8& dest_square)
{   // Update pieces:
    //  En passant target has to be the dest square of the attacking pawn opposed to the square of the attacked pawn due to how FEN works
    pieces[Piece::PAWN] = Utils::clear_bit(pieces[Piece::PAWN], src_square);
    if(turn == Turn::WHITE) {
        pieces[Piece::PAWN] = Utils::clear_bit(pieces[Piece::PAWN], dest_square - 8);
    }
    else {
        pieces[Piece::PAWN] = Utils::clear_bit(pieces[Piece::PAWN], dest_square + 8);
    }
    pieces[Piece::PAWN] |= 1ULL << dest_square;

    // Update colours:
    colours[turn] = Utils::clear_bit(colours[turn], src_square);
    if(turn == Turn::WHITE) {
        colours[Turn::BLACK] = Utils::clear_bit(colours[Turn::BLACK], dest_square - 8);   
    }
    else {
        colours[Turn::WHITE] = Utils::clear_bit(colours[Turn::WHITE], dest_square + 8);
    }
    colours[turn] |= 1ULL << dest_square;

    // Zobrist: Update src, capture and desc square
    zobrist_key ^= Zobrist::get_piece(Piece::PAWN + (6 * turn), src_square); // update src square
    if(turn == Turn::WHITE) {
        zobrist_key ^= Zobrist::get_piece(Piece::PAWN + 6, dest_square - 8); // update relevant capture square
    }
    else {
        zobrist_key ^= Zobrist::get_piece(Piece::PAWN, dest_square + 8); // update relevant capture square
    }
    zobrist_key ^= Zobrist::get_piece(Piece::PAWN + (6 * turn), dest_square); // update dest square
}

/**
 * @brief Updates internal board state of pieces and colours for castling
 * 
 * @param src_square 
 * @param dest_square 
 */
void Position::handle_castling(const u8& src_square, const u8& dest_square) {
    // Update King:
    handle_move(Piece::KING, Piece::KING, Piece::INVALID, src_square, dest_square);

    // Update Rook:
    if(dest_square == 2) { // white long castle
        handle_move(Piece::ROOK, Piece::ROOK, Piece::INVALID, 0, 3);
    }
    else if(dest_square == 6) { // white short castle
        handle_move(Piece::ROOK, Piece::ROOK, Piece::INVALID, 7, 5);
    }
    else if(dest_square == 58) { // black long castle
        handle_move(Piece::ROOK, Piece::ROOK, Piece::INVALID, 56, 59);
    }
    else { // if dest_square == 62 // black short castle
        handle_move(Piece::ROOK, Piece::ROOK, Piece::INVALID, 63, 61);
    }
}

// Applies changes to a copy of the Position object that calls it. Returns said copy.
void Position::make_move(Move& move) // simpler than make and unmake but slightly slower.
{
    Move_Flag flag = move.get_flag();
    u8 src_square = move.get_src_square();
    u8 dest_square = move.get_dest_square();
    Piece moved_piece_type = get_piece_type_from_square(src_square);
    Piece captured_piece_type = get_piece_type_from_square(dest_square);
    half_move_clock++;
    full_move_counter = half_move_clock / 2; // ints floor divide in C++

    switch(flag) {
        case NULL_FLAG:
            std::cerr << "Error: Flag type for a move was invalid.\n";
            std::exit(-1);

        case ROOK_FLAG:
            handle_move(Piece::ROOK, Piece::ROOK, captured_piece_type, src_square, dest_square);
            // if(castling_rights == 0) { // if already cant castle, ignore removing castling
            //     break;
            // }
            zobrist_key ^= Zobrist::get_castling_rights(castling_rights); // remove old castling rights value
            //std::cout << std::to_string(castling_rights ^ Zobrist::get_castling_rights(0)) << std::endl;
            // no need to check colour as this implies if any rook got to src square that they cant castle that side
            if(src_square == 0) {
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
            zobrist_key ^= Zobrist::get_castling_rights(castling_rights); // add updated castling rights value
            break;

        case KING_FLAG:
            handle_move(Piece::KING, Piece::KING, captured_piece_type, src_square, dest_square);
            zobrist_key ^= Zobrist::get_castling_rights(castling_rights); // remove old castling rights
            if(turn == Turn::WHITE) {
                if(get_wlcr()) { remove_wlcr(); }
                if(get_wscr()) { remove_wscr(); }
            }
            else {
                if(get_blcr()) { remove_blcr(); }
                if(get_bscr()) { remove_bscr(); }
            }
            zobrist_key ^= Zobrist::get_castling_rights(castling_rights); // add new castling rights
            break;

        case PAWN_TWO_FORWARD_FLAG:
            handle_move(Piece::PAWN, Piece::PAWN, Piece::INVALID, src_square, dest_square);
            if(en_passant_target != Utils::NULL_EN_PASSANT) {
                zobrist_key ^= Zobrist::get_en_passant(en_passant_target); // remove old value
            }
            if(turn == Turn::WHITE) {
                en_passant_target = dest_square - 8; // set the target to the destination square of a pawn taking with en passant
            }
            else {
                en_passant_target = dest_square + 8;
            }
            zobrist_key ^= Zobrist::get_en_passant(en_passant_target); // add new value
            break;

        case EN_PASSANT_FLAG:
            handle_en_passant(src_square, dest_square);
            break;

        case KNIGHT_PROMOTION_FLAG:
            // dest square piece type changes to knight for promotion
            handle_move(Piece::PAWN, Piece::KNIGHT, captured_piece_type, src_square, dest_square);
            break;

        case BISHOP_PROMOTION_FLAG:
            handle_move(Piece::PAWN, Piece::BISHOP, captured_piece_type, src_square, dest_square);
            break;

        case ROOK_PROMOTION_FLAG:
            handle_move(Piece::PAWN, Piece::ROOK, captured_piece_type, src_square, dest_square);
            break;

        case QUEEN_PROMOTION_FLAG:
            handle_move(Piece::PAWN, Piece::QUEEN, captured_piece_type, src_square, dest_square);
            break;

        case CASTLING_FLAG:
            handle_castling(src_square, dest_square);
            zobrist_key ^= Zobrist::get_castling_rights(castling_rights); // remove old castling rights
            if(turn == Turn::WHITE) {
                if(get_wlcr()) { remove_wlcr(); }
                if(get_wscr()) { remove_wscr(); }
            }
            else {
                if(get_blcr()) { remove_blcr(); }
                if(get_bscr()) { remove_bscr(); }
            }
            zobrist_key ^= Zobrist::get_castling_rights(castling_rights); // add new castling rights
            break;

        default: // Pawn, knight, bishop, queen move
            handle_move(moved_piece_type, moved_piece_type, captured_piece_type, src_square, dest_square);
            break;
    }

    // Half move clock restarts whenever a pawn move or capture has occured
    if(moved_piece_type == Piece::PAWN || captured_piece_type != Piece::INVALID) {
        half_move_clock = 0;
        // reset search stack for 3 move repetitions
    }

    // Unless we have caused a new en passant, we should reset en passant square, only if isnt already null
    if(flag != PAWN_TWO_FORWARD_FLAG) {
        if(en_passant_target != Utils::NULL_EN_PASSANT) {
            zobrist_key ^= Zobrist::get_en_passant(en_passant_target); // remove old en passant value
            en_passant_target = Utils::NULL_EN_PASSANT;
            // zobrist_key ^= Zobrist::get_en_passant(en_passant_target); // add new en passant value
        }
    }
    // Flip the turn
    turn = !turn;
    zobrist_key ^= Zobrist::get_side_to_move();
}

bool Position::equals(const Position& posb) const {
    if (get_white_pieces() != posb.get_white_pieces()) { return false; }

    else if (get_black_pieces() != posb.get_black_pieces()) { return false; }

    else if (get_pawns() != posb.get_pawns()) { return false; }

    else if (get_knights() != posb.get_knights()) { return false; }

    else if (get_bishops() != posb.get_bishops()) { return false; }

    else if (get_rooks() != posb.get_rooks()) { return false; }

    else if (get_queens() != posb.get_queens()) { return false; }
    
    else if (get_kings() != posb.get_kings()) { return false; }
    
    else if (get_castling_rights() != posb.get_castling_rights()) { return false; }
    
    else if (get_en_passant_target() != posb.get_en_passant_target()) { return false; }

    else if(turn != posb.get_turn()) {  return false; }
    
    // If all checks pass, the positions are equal
    return true;
}

std::string Position::equals_with_debugging(const Position& posb) const {
    std::string ret_string = "Error: ";
    if (get_white_pieces() != posb.get_white_pieces()) {
        ret_string += "White pieces were not equal.\n";
    }
    if (get_black_pieces() != posb.get_black_pieces()) {
        ret_string += "Black pieces were not equal.\n";
    }
    if (get_pawns() != posb.get_pawns()) {
        ret_string += "Pawns were not equal.\n";
    }
    if (get_knights() != posb.get_knights()) { ret_string += "Knights were not equal.\n"; }

    if (get_bishops() != posb.get_bishops()) { ret_string += "Bishops were not equal.\n"; }

    if (get_rooks() != posb.get_rooks()) { ret_string += "Rooks were not equal.\n"; }

    if (get_queens() != posb.get_queens()) { ret_string += "Queens were not equal.\n"; }
    
    if (get_kings() != posb.get_kings()) { ret_string += "Kings were not equal.\n"; }
    
    if (get_castling_rights() != posb.get_castling_rights()) { ret_string += "Castling rights were not equal.\n"; }
    
    if (get_en_passant_target() != posb.get_en_passant_target()) { ret_string += "En passant targets were not equal.\n"; }

    if (turn != posb.get_turn()) {  ret_string += "Turns were not equal"; }
    
    if(ret_string == "Error: ") {
        return "The two positions are equal.\n";
    }
    return ret_string;
}

std::string Position::zobrist_equals_with_debugging(const u64& zobrist_key_b) const {
    if (zobrist_key == zobrist_key_b) {
        return "The two zobrist keys are equal.\n";
    }
    
    std::string ret = "";
    if (zobrist_key == (zobrist_key_b ^ Zobrist::get_side_to_move())) {
        return "Error: The turn within the zobrist keys are not equal.\n";
    }
    for(int i = 0; i < 16; i++) {
        if(zobrist_key == (zobrist_key_b ^ Zobrist::get_castling_rights(static_cast<u8>(i)))) {
            return "Error: The castling rights within the zobrist keys are not equal.\nTrue castling rights: " + std::to_string(castling_rights) + "\nZobrist castling rights ^ that succeeded: " + std::to_string(i) + "\n";
        }
    }

    for(int j = 0; j < 64; j++) {
        if(zobrist_key == (zobrist_key_b ^ Zobrist::get_en_passant(j))) {
            return "Error: The en passant targets within the zobrist keys are not equal.\n";
        }
    }
    for(int type = 0; type < 12; type++) {
        for(int square = 0; square < 64; square++) {
            if(zobrist_key == (zobrist_key_b ^ Zobrist::get_piece(type, static_cast<u8>(square)))) {
                // will only become true if we can find the exact disrepency for a specific piece type and square.
                // otherwise computation would be too expensive to calculate all possibilities.
                ret += "Error: The following piece type on the following square within the zobrist keys are not equal.\n";
                ret += "Piece type: " + std::to_string(type);
                ret += "\nSquare: " + std::to_string(square);
                ret += "\n";
                return ret;
            }
        }
    }
    ret += "Error: The keys are not equal for more than one reason. Reasons could be any combination of errors, not just a single error. This could be errors with pieces or any combinations of pieces and/or other values.\n";
    ret += "Zobrist Key 1: " + std::to_string(zobrist_key) + "\n";
    ret += "Zobrist Key 2: " + std::to_string(zobrist_key_b) + "\n";
    return ret;
}

// Uses reference for data type larger than u64
std::array<u64, 6> Position::get_pieces() const { return pieces; }
std::array<u64, 2> Position::get_colours() const { return colours; }
u64 Position::get_en_passant_target() const { return en_passant_target; }
u8 Position::get_half_move_clock() const { return half_move_clock; }
u8 Position::get_full_move_counter() const { return full_move_counter; }
u8 Position::get_castling_rights() const { return castling_rights; }
Turn Position::get_turn() const { return turn; }
Turn Position::get_opp_turn() const { if(turn == Turn::WHITE) { return Turn::BLACK; } return Turn::WHITE; }
u64 Position::get_pawns() const { return pieces[Piece::PAWN]; }
u64 Position::get_white_pawns() const { return pieces[Piece::PAWN] & colours[Turn::WHITE]; }
u64 Position::get_black_pawns() const { return pieces[Piece::PAWN] & colours[Turn::BLACK]; }

u64 Position::get_knights() const { return pieces[Piece::KNIGHT]; }
u64 Position::get_white_knights() const { return pieces[Piece::KNIGHT] & colours[Turn::WHITE]; }
u64 Position::get_black_knights() const { return pieces[Piece::KNIGHT] & colours[Turn::BLACK]; }

u64 Position::get_bishops() const { return pieces[Piece::BISHOP];}
u64 Position::get_white_bishops() const { return pieces[Piece::BISHOP] & colours[Turn::WHITE]; }
u64 Position::get_black_bishops() const { return pieces[Piece::BISHOP] & colours[Turn::BLACK]; }

u64 Position::get_rooks() const { return pieces[Piece::ROOK]; }
u64 Position::get_white_rooks() const { return pieces[Piece::ROOK] & colours[Turn::WHITE]; }
u64 Position::get_black_rooks() const { return pieces[Piece::ROOK] & colours[Turn::BLACK]; }

u64 Position::get_queens() const { return pieces[Piece::QUEEN];}
u64 Position::get_white_queen() const { return pieces[Piece::QUEEN] & colours[Turn::WHITE]; }
u64 Position::get_black_queen() const { return pieces[Piece::QUEEN] & colours[Turn::BLACK]; }

u64 Position::get_kings() const { return pieces[Piece::KING]; }
u64 Position::get_white_king() const { return pieces[Piece::KING] & colours[Turn::WHITE]; }
u64 Position::get_black_king() const { return pieces[Piece::KING] & colours[Turn::BLACK]; }

u64 Position::get_white_pieces() const { return colours[Turn::WHITE]; } 
u64 Position::get_black_pieces() const { return colours[Turn::BLACK]; } 
u64 Position::get_pieces_from_current_turn() const { if(turn == Turn::WHITE) { return get_white_pieces(); } return get_black_pieces(); }

u64 Position::get_board() const { return colours[Turn::WHITE] | colours[Turn::BLACK]; }

bool Position::get_wscr() const { return castling_rights & (1 << Castling_Rights::WHITE_SHORT); }
bool Position::get_wlcr() const { return castling_rights & (1 << Castling_Rights::WHITE_LONG); }
bool Position::get_bscr() const { return castling_rights & (1 << Castling_Rights::BLACK_SHORT); }
bool Position::get_blcr() const { return castling_rights & (1 << Castling_Rights::BLACK_LONG); }

u64 Position::get_zobrist_key() const { return zobrist_key; }

void Position::remove_wscr() { castling_rights = castling_rights & ~(1 << Castling_Rights::WHITE_SHORT); }
void Position::remove_wlcr() { castling_rights = castling_rights & ~(1 << Castling_Rights::WHITE_LONG); }
void Position::remove_bscr() { castling_rights = castling_rights & ~(1 << Castling_Rights::BLACK_SHORT); }
void Position::remove_blcr() { castling_rights = castling_rights & ~(1 << Castling_Rights::BLACK_LONG); }

// Returns 0 for white and 1 for black
int Position::get_colour_value_from_square(u8 square) { return (get_black_pieces() & (1ULL << square)) ? 1 : 0; }

void Position::set_turn(Turn turn) {this->turn = turn;}

Move Position::board_notation_to_move(std::string board_notation) {
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
        return Utils::encode_move(get_piece_type_from_square(src_square), src_square, dest_square, get_en_passant_target());
    }
}