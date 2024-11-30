#include "Magics.hpp"

#include <random>
#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>

std::vector<u64> bishop_magics_table[64] = {};
std::vector<u64> rook_magics_table[64] = {};

u64 rook_magics[64] =
{
    396326115742859266ULL, 18023881798320192ULL, 1225014287313027080ULL, 36033745626597376ULL,
    11673339588585463952ULL, 144119594712854529ULL, 108088590088536320ULL, 9403516316708503680ULL,
    36310413737984192ULL, 141012375183360ULL, 1153625329488628096ULL, 72339106075512837ULL,
    310889128965179392ULL, 9223935004021622929ULL, 9570390867259904ULL, 288371270406390528ULL,
    36030171477704787ULL, 579416790388514816ULL, 1733886406830358544ULL, 4504149517406336ULL,
    9224499040569264128ULL, 592364639384240640ULL, 5188221537790460290ULL, 612491748412768385ULL,
    288300751338414208ULL, 162147179308777632ULL, 1689408207079458ULL, 9024793589842305ULL,
    10133651065677826ULL, 1297318180544577544ULL, 11276642796700177ULL, 1244120505162678412ULL,
    2485992354431893696ULL, 54043746643223296ULL, 162446248090013696ULL, 9227876187051657216ULL,
    9223380835103671298ULL, 613054700454937600ULL, 18298107204731184ULL, 1154052921432735828ULL,
    1153062516975370240ULL, 4616207347751075840ULL, 2305985946801274928ULL, 9259682652449996872ULL,
    1731634610976161800ULL, 649081313751597065ULL, 13835621042346917921ULL, 576461044370767877ULL,
    4611756405970436736ULL, 4899986765623853184ULL, 4764808474482180160ULL, 11751026127299182720ULL,
    2882492879665102976ULL, 1266757672239232ULL, 3314704370179834880ULL, 1477462994610881024ULL,
    1229554166802972673ULL, 55204838154257154ULL, 4611967769624715265ULL, 2306146616167911425ULL,
    23080982725025794ULL, 281483973755025ULL, 36099724243570700ULL, 2815884718539778ULL

};

u64 bishop_magics[64] =
{
    36627516013871264ULL, 146384585880543241ULL, 4508282219659264ULL, 181274285371818242ULL,
    5077614490292228ULL, 1171078909425483904ULL, 577591059955056640ULL, 6053401949811804161ULL,
    36068416214352385ULL, 9011722564403456ULL, 144238613792694340ULL, 9367489563541405732ULL,
    567365725061120ULL, 4755802374868828432ULL, 4508206114539520ULL, 2305983893285636096ULL,
    9516106031424472064ULL, 182405817987162624ULL, 9570153536946432ULL, 146371437510270977ULL,
    4630826325704310916ULL, 9223442440093434884ULL, 563272118050816ULL, 36099170125875968ULL,
    586631256402825216ULL, 6795188152371264ULL, 18313466226671648ULL, 288819714451324936ULL,
    676393186620358656ULL, 4513503891195904ULL, 28147776919962688ULL, 5911678795919935504ULL,
    23649674774974464ULL, 18383868779301120ULL, 288547482180255873ULL, 128515403001889296ULL,
    2253055018139904ULL, 2451093468815591437ULL, 1200211826369921154ULL, 10412604090483868164ULL,
    5190684581560452117ULL, 149540296460293ULL, 9148212698945536ULL, 2314850346326820865ULL,
    217360258970977285ULL, 282643258257536ULL, 9241953788229846024ULL, 2306973324349079616ULL,
    201536641944256520ULL, 25618930232197320ULL, 1188950868722450432ULL, 1154092485599428609ULL,
    7854366060131516680ULL, 2308113509594234944ULL, 5333564064108970001ULL, 1154050720429801610ULL,
    565150134571266ULL, 229756153125473537ULL, 1169309323264ULL, 2814761311963136ULL,
    9232379237187781376ULL, 1164251147877516544ULL, 19246212779639808ULL, 81100270007796240ULL
};

// Initialises magic tables
void Magics::init() {
	for(int square = 0; square < 64; square++) {
    	bishop_magics_table[square] = Magics::create_magic_table(Piece::BISHOP, bishop_magics[square], square);
	    rook_magics_table[square] = Magics::create_magic_table(Piece::ROOK, rook_magics[square], square);
    }
}

// Attempts to make the table used for the precomputations of magic attacks
std::vector<u64> Magics::create_magic_table(Piece piece_type, u64 magic_number, int square) {
    MagicEntry magic;
    magic.mask = get_relevant_blocker_squares(piece_type, square); // Bitboard of spaces where blockers would be an issue if occupied
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);
    magic.magic_number = magic_number;
    std::vector<u64> table(1 << magic.index_bits, 0ULL);
    u64 moves;
    for(u64 blockers : get_blocker_combinations(magic.mask)) {
        if(piece_type == Piece::BISHOP) { moves = pseudo_legalise_bishop_attacks_slow(square, blockers); }
        else if(piece_type == Piece::ROOK) { moves = pseudo_legalise_rook_attacks_slow(square, blockers); }
        // else { moves = pseudo_legalise_rook_attacks_slow(square, blockers) | pseudo_legalise_bishop_attacks_slow(square, blockers); }
        // Calculate the table index based on the magic number
        size_t index = get_magic_index(magic, blockers);
        // if empty
        if(table[index] == 0ULL) { table[index] = moves; }
        // if collision
        else if (table[index] != moves) { return {}; }
    }
    return table;
}

 size_t Magics::get_magic_index(MagicEntry magic, u64 blockers) {
   blockers &= magic.mask; // ands this blocker combination with the block attack mask
    // Perform the magic multiplication (wrapping multiply equivalent)
    uint64_t hash = blockers * magic.magic_number;
    // Calculate the index by shifting the hash
    size_t index = static_cast<size_t>(hash >> (64 - magic.index_bits));
    return index;
 }

// Gets squares where pieces are that block the sliding piece from moving past it
u64 Magics::get_blockers(Piece piece_type, int square, u64 board) {
    if(piece_type == Piece::BISHOP) { return Utils::BISHOP_ATTACKS_NO_EDGES[square] & board; }
    else if(piece_type == Piece::ROOK) { return Utils::ROOK_ATTACKS_NO_EDGES[square] & board; }
    else { return Utils::QUEEN_ATTACKS[square] & board; }
    return 0ULL;
}

// Gets squares where if a piece were on it, it would block the sliding piece from moving past it
u64 Magics::get_relevant_blocker_squares(Piece piece_type, int square) {
    if(piece_type == Piece::BISHOP) { return Utils::BISHOP_ATTACKS_NO_EDGES[square]; }
    else if(piece_type == Piece::ROOK) { return Utils::ROOK_ATTACKS_NO_EDGES[square]; }
    else { return Utils::QUEEN_ATTACKS_NO_EDGES[square]; }
    return 0ULL;
}

std::vector<u64> Magics::get_blocker_combinations(u64 blockers) {
    std::vector<u64> blocker_combinations;
    // Start from the full mask and generate all subsets
    u64 subset = blockers;
    do {
        blocker_combinations.push_back(subset);
        subset = (subset - 1) & blockers;  // Move to the next subset
    } while (subset != blockers);  // Stop when we cycle back to the full mask
    return blocker_combinations;
}

// Gets rook attacks dependent on blockers (used to precompute attacks which correspond to the magic indexes)
u64 Magics::pseudo_legalise_rook_attacks_slow(int square, u64 blockers) {// Shift in all directions until meet certain criteria
    u64 attacks_after_blockers = 0;
    int temp_square = 0; 
    u64 temp_bb;
    for(temp_square = square + 1; (temp_square) % 8 != 0; temp_square++) { // Checking Right
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb)) {
            break;
        }
    }
    for(temp_square = square - 1; (temp_square + 1) % 8 != 0 && temp_square >= 0; temp_square--) { // Checking Left
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb)) {
            break;
        }
    }   
    for(temp_square = square; temp_square < 64; temp_square += 8) { // Checking Up
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb)) {
            break;
        }
    }
    for(temp_square = square; temp_square >= 0; temp_square -= 8) { // Checking Down
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb)) {
            break;
        }
    }
    attacks_after_blockers ^= 1ULL << square; // Remove current square
    return attacks_after_blockers;
}

// Gets bishop attacks dependent on blockers (used to precompute attacks which correspond to the magic indexes)
u64 Magics::pseudo_legalise_bishop_attacks_slow(int square, u64 blockers) {
    // Shift in all directions until meet certain criteria
    u64 attacks_after_blockers = 0;
    int temp_square = 0; 
    u64 temp_bb;

    for (temp_square = square + 9; temp_square < 64 && (temp_square % 8 != 0); temp_square += 9) { // Checking Right Up
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }
    for (temp_square = square + 7; temp_square < 64 && ((temp_square + 1) % 8 != 0); temp_square += 7) { // Checking Left Up
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }   
    for (temp_square = square - 7; temp_square >= 0 && (temp_square % 8 != 0); temp_square -= 7) { // Checking Right Down
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }
    for (temp_square = square - 9; temp_square >= 0 && ((temp_square + 1) % 8 != 0); temp_square -= 9) { // Checking Left Down
        temp_bb = 1ULL << temp_square;
        attacks_after_blockers |= temp_bb;
        if((blockers & temp_bb) && (temp_square != square)) {
            break;
        }
    }
    attacks_after_blockers ^= 1ULL << square; // Remove current square
    return attacks_after_blockers;
}