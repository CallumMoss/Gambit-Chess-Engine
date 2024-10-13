#include "Magics.hpp"
#include "Position.hpp"

#include <random>
#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>

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

    // Hash tables that contain all combinations for rook attacks for all combinations of blockers. One per square
    extern u64 bishopAttacksTable[64][1ULL << 9ULL]; // At any given time, a bishop can be blocked by up to 9 squares (think about a rook on a1 for example)
    extern u64 rookAttacksTable[64][1ULL << 12ULL]; // At any given time, a rook can be blocked by up to 12 squares (think about a rook on a1 for example)

    u64 rook_magics[64]; // magic number used for each of the squares
    



    // u64 key = (blockers * magic.magic_number?) >> ROOK_SHIFTS[sq];
    // rookAttacksTable[sq][key] = rookAttacksSlow(sq, blockersArrangement);



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
Final_Magic find_magic(int square) {
    MagicEntry magic;
    magic.mask = Magics::get_relevant_blocker_squares(square); // Bitboard of spaces where blockers would be an issue if occupied (get_blockers gets the occupied)
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);

    Final_Magic successful_magic;
    const int index_bits = ROOK_SHIFTS[square];
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
        magic.magic_number = dis(gen) & dis(gen) & dis(gen);
        magic_table = Magics::create_magic_table(magic, square);
        successful_magic.magic = magic;
        if (magic_table.size() == ((std::size_t)(1 << magic.index_bits))) { // if full table (meaning no collisions or missed computations)
            successful_magic.table = magic_table;
            return successful_magic;
        }
    }
}

// Attempts to make the table used for the precomputations of magic attacks
bb_vector create_magic_table(MagicEntry magic, int square) {
    bb_vector table(1 << magic.index_bits, 0ULL);
    u64 moves;
    for(u64 blockers : Magics::get_blocker_combinations(magic.mask)) {
        moves = Magics::pseudo_legalise_rook_attacks_slow(square, blockers);
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

 size_t get_magic_index(MagicEntry magic, u64 blockers) {
    blockers &= magic.mask; // ands this blocker combination with the block attack mask
    // Perform the magic multiplication (wrapping multiply equivalent)
    uint64_t hash = blockers * magic.magic_number;

    // Calculate the index by shifting the hash
    size_t index = static_cast<size_t>(hash >> (64 - magic.index_bits));

    return index;
 }
 
// Gets squares where pieces are that block the sliding piece from moving past it
u64 get_blockers(int square, u64 board) {
    return Utils::ROOK_ATTACKS_NO_EDGES[square] & board; // @s with get_board()
}

// Gets squares where if a piece were on it, it would block the sliding piece from moving past it
u64 get_relevant_blocker_squares(int square) {
    return Utils::ROOK_ATTACKS_NO_EDGES[square];
}

bb_vector get_blocker_combinations(u64 blockers) {
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
u64 pseudo_legalise_rook_attacks_slow(int square, u64 blockers) {
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