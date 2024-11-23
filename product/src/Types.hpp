#ifndef TYPES_HPP
#define TYPES_HPP

// Define a macro "gambit debug" to replace anywhere with GAMBIT_DEBUG with the value to the right of it.
// 1 if want to check asserts, 0 for ignore
#define GAMBIT_DEBUG 1
#if GAMBIT_DEBUG == 1
#else
#define NDEBUG
#endif

// check that types is included everywhere

#include <cassert>
#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <list>

using u8 = uint8_t;
using u64 = uint64_t;
using u16 = uint16_t;
using bb_vector = std::vector<u64>;

#endif // #ifndef TYPES_HPP