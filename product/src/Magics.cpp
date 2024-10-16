#include "Magics.hpp"
#include <random>
#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>

//Final_Magic rook_magics_table[64] = {};
Final_Magic bishop_magics_table[64] = {};

bb_vector rook_magics_table[64] = {};
//bb_vector bishop_magics_table[64] = {};

u64 rook_magics[64] = {
    36029621661147156ULL, 738607931611693056ULL, 9259418428207800320ULL, 72075190529500168ULL, 9367497464672947712ULL, 144117387235758084ULL, 13979195278721286408ULL, 36028938752901248ULL,
    74450156036063392ULL, 422487412179584ULL, 9223653649276011076ULL, 612630355538614272ULL, 9801240237058689024ULL, 72620561741647881ULL, 54324713455943756ULL, 562952168161554ULL,
    1152926452417576960ULL, 9272911907652632642ULL, 9246033521527496704ULL, 284774049513736ULL, 9808981275724220416ULL, 2815300101751808ULL, 9800197827036447240ULL, 9299091004630009956ULL,
    1152930852603174985ULL, 9234666221369053186ULL, 323265009582145ULL, 4918001582740733960ULL, 289356310552543296ULL, 18577357053366309ULL, 18295877781291012ULL, 304315612856452ULL,
    288231759135383937ULL, 633323042906241ULL, 615023683391463424ULL, 288247970493650944ULL, 2324139226381166592ULL, 288637197609862144ULL, 1153203031844585984ULL, 353013596162180ULL,
    4957636034564ULL, 11565331804554543104ULL, 11601554802278989888ULL, 595038444500156481ULL, 2312599834609090568ULL, 18016597599879296ULL, 9232942222637400067ULL, 2307074775777935364ULL,
    17597555319360ULL, 1152998471242908160ULL, 72374254728937600ULL, 2254067556974720ULL, 9224497971121651840ULL, 4630263985433282048ULL, 2418450597486791680ULL, 1171007372722971136ULL,
    144115785080525314ULL, 18156261281402946ULL, 844460107767809ULL, 9433845737718017ULL, 2306407144847057026ULL, 10414866196203529ULL, 281517926417409ULL, 108091889697235978ULL
};

u64 bishop_magics[64] = {
    153131183432040961ULL, 2199562813440ULL, 216190374366942208ULL, 80814256751620ULL, 19142669305381122ULL, 9663709184ULL, 576531172587225092ULL, 4629982469595286530ULL,
    72339075465740288ULL, 4538786146943021ULL, 1152996273545174114ULL, 1099512152576ULL, 4503737636818948ULL, 297964604922011648ULL, 17935973220352ULL, 1164180521056468992ULL,
    4616189644092997776ULL, 1247497096785821824ULL, 4616260270267334656ULL, 40541330196664356ULL, 144260598539618578ULL, 577067682857222144ULL, 201474564ULL, 9148658901590336ULL,
    2328466560471044096ULL, 378344218860781568ULL, 1343230976ULL, 4521213330202770ULL, 4503703176348416ULL, 9367496021309931524ULL, 36732485014405121ULL, 9223380867307544720ULL,
    1158271799058769920ULL, 2919599230291018241ULL, 9851624520418440ULL, 2251834208043072ULL, 1153202980154115089ULL, 4612073081031361536ULL, 1441152155738734632ULL, 4720640ULL,
    40533496174758420ULL, 18102376721809664ULL, 2251799815818240ULL, 11673368858785219584ULL, 9232555742454612120ULL, 140741783322624ULL, 148619066882392160ULL, 9516141269215353088ULL,
    144396665200590852ULL, 351845927100416ULL, 9046817140637776ULL, 36029072433759241ULL, 600104788876591370ULL, 301742275094937632ULL, 72774614131935520ULL, 4400261365761ULL,
    1161102130153600ULL, 9225623837746413569ULL, 1180224663316403202ULL, 72057639136149504ULL, 1297036693219573760ULL, 1155173579333173256ULL, 667696577984351304ULL, 25334414498076032ULL
};


u64 ROOK_SHIFTS[64] = {
    52, 53, 53, 53, 53, 53, 53, 52, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    53, 54, 54, 54, 54, 54, 54, 53, 
    52, 53, 53, 53, 53, 53, 53, 52
};


// ARCHIVED:
// Initialises magic tables
void Magics::init()
{
    Final_Magic fm;
    int key;
    u64 attacks;
	for(int square = 0; square < 64; square++)
    {
    	//bishop_magics_table[square] = Magics::find_magic(Piece::BISHOP, square);
	    bishop_magics_table[square] = Magics::find_magic(Piece::BISHOP, square);
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
        magic_table = create_magic_table(piece_type, magic.magic_number, square);
        if (magic_table.size() == ((std::size_t)(1 << magic.index_bits))) { // if full table (meaning no collisions or missed computations)
            successful_magic.magic = magic;
            successful_magic.table = magic_table;
            return successful_magic;
        }
    }
}

// // Initialises magic tables
// void Magics::init()
// {
// 	for(int square = 0; square < 64; square++)
//     {
//     	//bishop_magics_table[square] = Magics::find_magic(Piece::BISHOP, square);
// 	    rook_magics_table[square] = Magics::create_magic_table(Piece::ROOK, rook_magics[square], square);
//         //rook_magics_table[square] = Magics::create_magic_table(Piece::ROOK, square);
//         //queen_magics_table
//     }
// }

// Attempts to make the table used for the precomputations of magic attacks
bb_vector Magics::create_magic_table(Piece piece_type, u64 magic_number, int square)
{
    MagicEntry magic;
    magic.mask = get_relevant_blocker_squares(piece_type, square); // Bitboard of spaces where blockers would be an issue if occupied (get_blockers gets the occupied)
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);
    magic.magic_number = magic_number;

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
            return {};
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
u64 Magics::get_blockers(Piece piece_type, int square, u64 board)
{
    if(piece_type == Piece::BISHOP)
    {
        return Utils::BISHOP_ATTACKS_NO_EDGES[square] & board;
    }
    else if(piece_type == Piece::ROOK)
    {
        return Utils::ROOK_ATTACKS_NO_EDGES[square] & board;
    }
    else
    {
        return Utils::QUEEN_ATTACKS[square] & board;
    }
    return 0ULL;
}

// Gets squares where if a piece were on it, it would block the sliding piece from moving past it
u64 Magics::get_relevant_blocker_squares(Piece piece_type, int square)
{
    if(piece_type == Piece::BISHOP)
    {
        return Utils::BISHOP_ATTACKS_NO_EDGES[square];
    }
    else if(piece_type == Piece::ROOK)
    {
        return Utils::ROOK_ATTACKS_NO_EDGES[square];
    }
    else
    {
        return Utils::QUEEN_ATTACKS_NO_EDGES[square];
    }
    return 0ULL;
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

// Gets rook attacks dependent on blockers (used to precompute attacks which correspond to the magic indexes)
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
    return attacks_after_blockers;
}

// Gets bishop attacks dependent on blockers (used to precompute attacks which correspond to the magic indexes)
u64 Magics::pseudo_legalise_bishop_attacks_slow(int square, u64 blockers)
{
    // Shift in all directions until meet certain criteria
    u64 attacks_after_blockers = 0;
    int temp_square = 0; 
    u64 temp_bb;

    for (temp_square = square + 9; temp_square < 64 && (temp_square % 8 != 0); temp_square += 9) 
    { // Checking Right Up
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }

    for (temp_square = square + 7; temp_square < 64 && ((temp_square + 1) % 8 != 0); temp_square += 7)
    { // Checking Left Up
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }   
    for (temp_square = square - 7; temp_square >= 0 && (temp_square % 8 != 0); temp_square -= 7) 
    { // Checking Right Down
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }
    for (temp_square = square - 9; temp_square >= 0 && ((temp_square + 1) % 8 != 0); temp_square -= 9) 
    { // Checking Left Down
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }
    attacks_after_blockers ^= 1ULL << square; // Remove current square
    return attacks_after_blockers;
}