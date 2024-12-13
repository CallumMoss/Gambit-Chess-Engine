#ifndef PERFT_HPP
#define PERFT_HPP

#include "../Types.hpp"
#include "../Position.hpp"

namespace Perft {
    static std::vector<std::string> Split(const std::string& str, const std::string& delimiter);
    static u64 test_perft(u8 depth, u64 expected_nodes, uint16_t test_number, const std::string& fen, const bool& output_perft);
    static bool run_perft_suite(const bool& output_perft);
    // A variation of perft which outputs total nodes per root move and total.
    u64 split_perft(int current_depth, const int& desired_depth, const bool& output_split, Move& last_move, Position pos);
}

#endif // #ifndef PERFT_HPP