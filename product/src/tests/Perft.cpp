#include <fstream>
#include <cstdint>
#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <chrono>

#include "../Position.hpp"
#include "../Types.hpp"

// Used AI assistance for this function as the problem seemed relatively trivial
std::vector<std::string> Split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

// Heavily inspired by the following implementation, but adapted for my own project: https://github.com/TiltedDFA/TDFA/tree/c26a01e29ba87c41af50700c2c8321e3e2667c8f
u64 test_perft(u8 depth, u64 expected_nodes, uint16_t test_number, const std::string& fen, const bool& output_perft) {
    Position pos(fen);

    auto start = std::chrono::high_resolution_clock::now();

    u64 total_nodes = pos.split_perft(depth, depth, output_perft);

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    const uint64_t nps = static_cast<uint64_t>(total_nodes) * 1'000'000 / duration;

    if(expected_nodes == total_nodes) {
        std::cout << "Test " << test_number << " passed at depth " << static_cast<int>(depth) << " with " << nps << " nps" << std::endl;
    }
    else {
        std::cout << "Test " << test_number << " *FAILED*. Exp " << expected_nodes << ", was " << static_cast<int>(total_nodes) << std::endl;
        std::exit(-1);
    }
    return nps;
}

bool run_perft_suite(const bool& output_perft) {
    std::fstream perft_file("../src/tests/perftsuite.epd", std::ios::in);

    if(!perft_file.is_open()){return false;}

    std::string line;

    uint32_t current_perft{1};

    while(std::getline(perft_file, line))
    {
        if(line[0] == '#') continue;
        
        std::vector<std::string> chunks = Split(line, ";");
        const std::string& fen = chunks[0];
        int depth{999999};
        u64 expected_nodes{0};

        for(int i = chunks.size() - 1; i > 0 ; i--)
        {
            if(std::stoull(chunks[i].substr(3)) > 100'000'000) continue;
            depth = (chunks[i])[1] - '0';
            expected_nodes = std::stoull(chunks[i].substr(3));
            break;
        }
        assert(depth < 7);
        test_perft(depth, expected_nodes, current_perft, fen, output_perft);
        current_perft++;
    }

    perft_file.close();
    return true;
}