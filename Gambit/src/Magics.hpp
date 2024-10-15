#ifndef MAGICS_HPP
#define MAGICS_HPP

#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "utils.hpp"

struct MagicEntry {
    u64 mask;
    u64 magic_number;
    int index_bits; // Could probably use a u8 for smaller data type but this keeps it simple
};

struct Final_Magic {
    bb_vector table;
    MagicEntry magic;
};
namespace Magics {
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