#ifndef MAGICS_HPP
#define MAGICS_HPP

#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <list>

using u8 = uint8_t;
using u64 = uint64_t;

namespace Magics {
    std::vector<u64> create_blocker_bitboards(u64 moves);
}
#endif // #ifndef MAGICS_HPP