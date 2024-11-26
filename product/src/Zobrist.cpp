#include "Zobrist.hpp"

#include <algorithm>
//Inspired by: https://www.chessprogramming.org/Zobrist_Hashing#Initialization

Zobrist::Zobrist() {
    assert(instance == nullptr);
    instance = this;
    // Initialize a Mersenne Twister pseudo-random number generator
    std::mt19937_64 gen(18446744073709551615ULL); // could probably use a different generator, check later. For now just picked a random seed
    // Define the range of random numbers (inclusive)
    std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);
    // by two more random values to cut down on the bits set.
    for(std::array<u64, 12>& I : pieces) {
        for(u64& II : I) {
            II = dis(gen);
        }
    }
    // Another way to fill the array, for learning purposes
    for(std::array<u64, 64>::iterator it = en_passant.begin(); it != en_passant.end(); ++it) {
        *it = dis(gen);
    }
    
    auto foo = [&dis, &gen](u64& elem)->void{elem = dis(gen);};

    std::for_each(castling_rights.begin(), castling_rights.end(), foo);

    side_to_move = dis(gen);
}