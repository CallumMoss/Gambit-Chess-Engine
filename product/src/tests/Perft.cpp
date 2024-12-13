#include <fstream>
#include <cstdint>
#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <chrono>

#include "Perft.hpp"


// Perft suite used: http://www.rocechess.ch/perft.html

// Used AI assistance for this function as the problem seemed relatively trivial
static std::vector<std::string> Perft::Split(const std::string& str, const std::string& delimiter) {
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
static u64 Perft::test_perft(u8 depth, u64 expected_nodes, uint16_t test_number, const std::string& fen, const bool& output_perft) {
    Position pos(fen);

    auto start = std::chrono::high_resolution_clock::now();

    Move null_move = Move(0, 0, Move_Flag::NULL_FLAG);
    u64 total_nodes = pos.split_perft(depth, depth, output_perft, null_move);

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

static bool Perft::run_perft_suite(const bool& output_perft) {
    std::fstream perft_file("../product/src/tests/perftsuite.epd", std::ios::in);

    if(!perft_file.is_open()){
        std::cerr << "Perft file could not open\n";
        return false;
    }

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

u64 Perft::split_perft(int current_depth, const int& desired_depth, const bool& output_split, Move& last_move, Position pos) // desired depth being the initial depth input
{
    Move null_move = Move(0, 0, NULL_FLAG);
    if(!last_move.equals(null_move)) {
        Position current_pos = pos;
        current_pos.recompute_zobrist_key(); // reset zobrist key then recalculate to see if the key has been updated properly based on the position
        u64 carried = pos.get_zobrist_key();
        u64 actual = current_pos.get_zobrist_key();
        if(actual != carried) {
            // std::cerr << equals_with_debugging(current_pos);
            // std::cerr << zobrist_equals_with_debugging(actual) << std::endl;
            std::cerr << "Last move applied: " << Utils::move_to_board_notation(last_move) << "\tMove Flag: " << static_cast<int>(last_move.get_flag()) << std::endl;
            std::cerr << "Board: " << std::endl;
            Utils::print_bb(pos.get_board());
            pos.print_position();
            std::cout << "Piece Type: " << pos.get_piece_type_from_square(last_move.get_dest_square()) << std::endl;
            std::cout << "Zobrist call log:" << std::endl;
            Zobrist::print_log();
            std::exit(-1);
        }
    }
    Zobrist::clear_log();

    if (current_depth == 0) {
        return 1ULL;
    }
    
    u64 sum = 0;
    u64 nodes = 0;
    std::vector<Move> moves = pos.generate_all_moves();
    for(Move& move : moves) {
        // Reset position
        // Create a copy of the current starting position at each depth
        Position new_position = pos;
        new_position.make_move(move); // apply move to current pos

        if(!new_position.is_legal(move)) { // if move isnt legal, don't count
            continue;
        }
        nodes = split_perft(current_depth - 1, desired_depth, output_split, move, new_position);
        sum += nodes;

        if(output_split) {
            if(current_depth == desired_depth) { // if finished recursion
                std::cout << Utils::move_to_board_notation(move) << ": " << nodes << std::endl;
            }
        }
    }
    return sum;
}