#include "Magics.hpp"
#include <random>
#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>

Final_Magic rook_magics_table[64] = {};
Final_Magic bishop_magics_table[64] = {};

// Initialises magic tables
void Magics::init(Position pos)
{
    Final_Magic fm;
    int key;
    u64 attacks;
	for(int square = 0; square < 64; square++)
    {
    	//bishop_magics_table[square] = Magics::find_magic(Piece::BISHOP, square);
	    rook_magics_table[square] = Magics::find_magic(Piece::ROOK, square);
        //rook_magics_table[square] = Magics::create_magic_table(Piece::ROOK, square);
        //queen_magics_table
    }
}

// Inspiration drawn from: https://analog-hors.github.io/site/magic-bitboards/
Final_Magic Magics::find_magic(Piece piece_type, int square)
{
    MagicEntry magic;
    magic.mask = get_relevant_blocker_squares(piece_type, square); // Bitboard of spaces where blockers would be an issue if occupied (get_blockers gets the occupied)
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);

    Final_Magic successful_magic;
    bb_vector magic_table;

    // Create a random device
    std::random_device rd;
    // Initialize a Mersenne Twister pseudo-random number generator
    std::mt19937_64 gen(rd());
    // Define the range of random numbers (inclusive)
    std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);

    while(true) {
        // Magics require a low number of active bits, so we AND
        // by two more random values to cut down on the bits set.
        magic.magic_number = dis(gen) & dis(gen) & dis(gen); // creates a random u64 magic number
        magic_table = create_magic_table(piece_type, magic, square);
        if (magic_table.size() == ((std::size_t)(1 << magic.index_bits))) { // if full table (meaning no collisions or missed computations)
            successful_magic.magic = magic;
            successful_magic.table = magic_table;
            return successful_magic;
        }
    }
}

// Attempts to make the table used for the precomputations of magic attacks
bb_vector Magics::create_magic_table(Piece piece_type, MagicEntry magic, int square)
{
    bb_vector table(1 << magic.index_bits, 0ULL);
    u64 moves;
    for(u64 blockers : get_blocker_combinations(magic.mask)) {
        moves = pseudo_legalise_rook_attacks_slow(square, blockers);
        // Calculate the table index based on the magic number
        size_t index = get_magic_index(magic, blockers);
        if(table[index] == 0ULL) { // if empty
            table[index] = moves;
        }
        else if (table[index] != moves) { // if collision
            return bb_vector(0);
        }
    }
    return table;
}

 size_t Magics::get_magic_index(MagicEntry magic, u64 blockers)
 {
    blockers &= magic.mask; // ands this blocker combination with the block attack mask
    // Perform the magic multiplication (wrapping multiply equivalent)
    uint64_t hash = blockers * magic.magic_number;

    // Calculate the index by shifting the hash
    size_t index = static_cast<size_t>(hash >> (64 - magic.index_bits));

    return index;
 }

// Gets squares where pieces are that block the sliding piece from moving past it
u64 Magics::get_blockers(int square, u64 board)
{
    return Utils::ROOK_ATTACKS_NO_EDGES[square] & board; // @s with get_board()
}

// Gets squares where if a piece were on it, it would block the sliding piece from moving past it
u64 Magics::get_relevant_blocker_squares(Piece piece_type, int square)
{
    if(piece_type == (Piece::BISHOP))
    {
        return Utils::BISHOP_ATTACKS_NO_EDGES[square];
    }
    return Utils::ROOK_ATTACKS_NO_EDGES[square];
}

bb_vector Magics::get_blocker_combinations(u64 blockers) {
    bb_vector blocker_combinations;
    // Start from the full mask and generate all subsets
    u64 subset = blockers;
    do {
        blocker_combinations.push_back(subset);
        subset = (subset - 1) & blockers;  // Move to the next subset
    } while (subset != blockers);  // Stop when we cycle back to the full mask

    return blocker_combinations;
}

// Gets rook attacks dependent on blockers (used to precompute attacks which correspond to the magic indexes, not used at runtime)
u64 Magics::pseudo_legalise_rook_attacks_slow(int square, u64 blockers)
{
    // Shift in all directions until meet certain criteria
    u64 attacks_after_blockers = 0;
    int temp_square = 0; 
    u64 temp_bb;

    for(temp_square = square + 1; (temp_square) % 8 != 0; temp_square++)
    { // Checking Right
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }

    for(temp_square = square - 1; (temp_square + 1) % 8 != 0 && temp_square >= 0; temp_square--)
    { // Checking Left
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }   
    for(temp_square = square; temp_square < 64; temp_square += 8)
    { // Checking Up
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }
    for(temp_square = square; temp_square > 0; temp_square -= 8)
    { // Checking Down
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }
    attacks_after_blockers ^= 1ULL << square; // Remove current square
    return attacks_after_blockers; // Actual location of blocking pieces
}