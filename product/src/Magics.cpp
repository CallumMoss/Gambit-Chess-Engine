#include "Magics.hpp"
#include <random>
#include <vector>
#include <sstream>
#include <iostream>
#include <array>
#include <cstdint>

// ARCHIVED:
//Final_Magic bishop_magics_table[64] = {};
//Final_Magic rook_magics_table[64] = {};

bb_vector bishop_magics_table[64] = {};
bb_vector rook_magics_table[64] = {};

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
    757946176078151696ULL, 2884557794747625473ULL, 1134979908108320ULL, 145246594198929664ULL, 9224515700849709056ULL, 3459334129636737024ULL, 54118032515531792ULL, 10414789155359762ULL,
    1441169541932550784ULL, 9011617232667137ULL, 1174312803696640ULL, 9548196367625617408ULL, 2308114609095081988ULL, 845043674382336ULL, 2558045284267459600ULL, 5764889548911943748ULL,
    873715937143955968ULL, 1249088230392833ULL, 290482245360226336ULL, 2257865012224016ULL, 1730510495437296896ULL, 18049625865003008ULL, 153932840568833ULL, 432556709136303136ULL,
    2307039278145882144ULL, 81355065167845378ULL, 9845160194936423937ULL, 168893782145237024ULL, 281682225676288ULL, 1155173854515037184ULL, 37159163696055296ULL, 172263235506947072ULL,
    2287010042610752ULL, 4902187092451001488ULL, 143625867494528ULL, 594580738288977156ULL, 3459890688873988352ULL, 2559665234250240ULL, 618124083110380552ULL, 11928351042703730820ULL,
    145291227439114ULL, 1130315737203200ULL, 1158835231168532481ULL, 4785212362594304ULL, 1170944978449367104ULL, 5282732409119311880ULL, 1161931186357971464ULL, 589342537975040ULL,
    568467376052611ULL, 9572363332747424ULL, 37154980403086336ULL, 9295548413615670273ULL, 9144019058688ULL, 595073423148154882ULL, 1183109040316428ULL, 13837877490925764868ULL,
    1729457029206516752ULL, 572313049302032ULL, 9367566940094140432ULL, 36028934470631969ULL, 9572348516762128ULL, 1161934236056487424ULL, 90086634561209225ULL, 20275372379865376ULL
};



// // ARCHIVED:
// // Initialises magic tables
// void Magics::init()
// {
//     Final_Magic fm;
//     int key;
//     u64 attacks;
// 	for(int square = 0; square < 64; square++)
//     {
//     	
// 	    bishop_magics_table[square] = Magics::find_magic(Piece::BISHOP, square);
//      rook_magics_table[square] = Magics::find_magic(Piece::ROOK, square);
// 
//     }
// }
// // Inspiration drawn from: https://analog-hors.github.io/site/magic-bitboards/
// Final_Magic Magics::find_magic(Piece piece_type, int square) {
//     MagicEntry magic;
//     magic.mask = get_relevant_blocker_squares(piece_type, square); // Bitboard of spaces where blockers would be an issue if occupied
//     magic.index_bits = Utils::count_number_of_1bs(magic.mask);
//     Final_Magic successful_magic;
//     bb_vector magic_table;
//     // Create a random device
//     std::random_device rd;
//     // Initialize a Mersenne Twister pseudo-random number generator
//     std::mt19937_64 gen(rd());
//     // Define the range of random numbers (inclusive)
//     std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
//     while(true) {
//         // Magics require a low number of active bits, so we AND
//         // by two more random values to cut down on the bits set.
//         magic.magic_number = dis(gen) & dis(gen) & dis(gen); // creates a random u64 magic number
//         magic_table = create_magic_table(piece_type, magic.magic_number, square);
//         if (magic_table.size() == ((std::size_t)(1 << magic.index_bits))) { // if full table (meaning no collisions or missed computations)
//             successful_magic.magic = magic;
//             successful_magic.table = magic_table;
//             return successful_magic;
//         }
//     }
// }

// Initialises magic tables
void Magics::init() {
	for(int square = 0; square < 64; square++) {
    	bishop_magics_table[square] = Magics::create_magic_table(Piece::BISHOP, bishop_magics[square], square);
	    rook_magics_table[square] = Magics::create_magic_table(Piece::ROOK, rook_magics[square], square);
    }
}

// Attempts to make the table used for the precomputations of magic attacks
bb_vector Magics::create_magic_table(Piece piece_type, u64 magic_number, int square) {
    MagicEntry magic;
    magic.mask = get_relevant_blocker_squares(piece_type, square); // Bitboard of spaces where blockers would be an issue if occupied
    magic.index_bits = Utils::count_number_of_1bs(magic.mask);
    magic.magic_number = magic_number;
    bb_vector table(1 << magic.index_bits, 0ULL);
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
u64 Magics::pseudo_legalise_rook_attacks_slow(int square, u64 blockers) {// Shift in all directions until meet certain criteria
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