#ifndef MAGICS_HPP
#define MAGICS_HPP

#include "Types.hpp"
#include "utils.hpp"
#include "Position.hpp"

#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <list>

// Holds the vector of attacks for a given piece on a given square
extern std::vector<u64> rook_magics_table[64];
extern std::vector<u64> bishop_magics_table[64];

// Holds the magic number for each square, that was found previously to generate the tables upon initialisation.
extern u64 rook_magics[64];
extern u64 bishop_magics[64];

namespace Magics {
    void init();
    u64 get_blockers(Piece piece_type, int square, u64 board);
    std::vector<u64> get_blocker_combinations(u64 blockers);
    std::vector<u64> get_attacks_after_blockers(int square, u64 attacks, std::vector<u64> blocking_squares);
    u64 pseudo_legalise_rook_attacks_slow(int square, u64 attacks);
    u64 pseudo_legalise_bishop_attacks_slow(int square, u64 blockers);

    std::vector<u64> create_magic_table(Piece piece_type, u64 magic, int square);
    u64 get_relevant_blocker_squares(Piece piece_type, int square);
    size_t get_magic_index(MagicEntry magic, u64 blockers);
}
#endif // #ifndef MAGICS_HPP