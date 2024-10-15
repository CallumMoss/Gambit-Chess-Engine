#ifndef MAGICS_HPP
#define MAGICS_HPP

#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "utils.hpp"
#include "Position.hpp"

struct MagicEntry {
    u64 mask;
    u64 magic_number;
    int index_bits; // Could probably use a u8 for smaller data type but this keeps it simple
};

struct Final_Magic {
    bb_vector table;
    MagicEntry magic;
};

// Holds the magic tables for each square (which can be accessed using combination of blockers to find the index to get the attacks)
extern Final_Magic bishop_magics_table[64]; // Each index represents each square, which holds a final magic that holds the MagicEntry and table for that square
extern Final_Magic rook_magics_table[64];

// Holds the magic number for each square, that was found previously to generate the tables upon initialisation.
extern u64 rook_magics[64] = {
    36029621661147156, 738607931611693056, 9259418428207800320, 72075190529500168, 9367497464672947712, 144117387235758084, 13979195278721286408, 36028938752901248,
    74450156036063392, 422487412179584, 9223653649276011076, 612630355538614272, 9801240237058689024, 72620561741647881, 54324713455943756, 562952168161554,
    1152926452417576960, 9272911907652632642, 9246033521527496704, 284774049513736, 9808981275724220416, 2815300101751808, 9800197827036447240, 9299091004630009956,
    1152930852603174985, 9234666221369053186, 323265009582145, 4918001582740733960, 289356310552543296, 18577357053366309, 18295877781291012, 304315612856452,
    288231759135383937, 633323042906241, 615023683391463424, 288247970493650944, 2324139226381166592, 288637197609862144, 1153203031844585984, 353013596162180,
    4957636034564, 11565331804554543104, 11601554802278989888, 595038444500156481, 2312599834609090568, 18016597599879296, 9232942222637400067, 2307074775777935364,
    17597555319360, 1152998471242908160, 72374254728937600, 2254067556974720, 9224497971121651840, 4630263985433282048, 2418450597486791680, 1171007372722971136,
    144115785080525314, 18156261281402946, 844460107767809, 9433845737718017, 2306407144847057026, 10414866196203529, 281517926417409, 108091889697235978
};

namespace Magics {
    void init(Position pos);
    u64 get_blockers(int square, u64 board);
    bb_vector get_blocker_combinations(u64 blockers);
    bb_vector get_attacks_after_blockers(int square, u64 attacks, bb_vector blocking_squares);
    u64 pseudo_legalise_rook_attacks_slow(int square, u64 attacks);

    Final_Magic find_magic(Piece piece_type, int square);
    bb_vector create_magic_table(Piece piece_type, MagicEntry magic, int square);
    u64 get_relevant_blocker_squares(Piece piece_type, int square);
    size_t get_magic_index(MagicEntry magic, u64 blockers);
}
#endif // #ifndef MAGICS_HPP