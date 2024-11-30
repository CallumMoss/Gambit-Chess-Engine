#include "Search.hpp"

#include <random>
#include <cmath>

Search::Search(){}

Search::Search(std::vector<u64> game_history_stack) { zobrist_key_stack = game_history_stack; }

int Search::negamax(int depth, int ply, const Position& pos, Timer& timer) {
    if(depth == 0) {
        return Evaluation::evaluate(pos);
    }
    int best_score = -INT_MAX;
    std::vector<Move> moves = pos.generate_all_moves();
    for(Move move : moves) {
        if(timer.is_out_of_time()) {
            return best_score; // could return anything as this score wont be used
        }
        // reset position
        Position new_position = pos;
        new_position.make_move(move);
        if(!new_position.is_legal(move)) {
            continue;
        }

        if(!has_found_a_legal_move) { has_found_a_legal_move = true; }
        int score = -negamax(depth - 1, ply + 1, new_position, timer);

        // Checking for repetition //
        // if(is_repetition(ply, pos)) {
        //     score = Utils::DRAW_SCORE;
        //     std::cout << "Repetition Detected!\n";
        //     std::cout << Utils::move_to_board_notation(move) << std::endl;
        //     //std::exit(-1);
        // }
        // std::cout << "Not repetition\n";
        // std::cout << Utils::move_to_board_notation(move) << std::endl;
        // ----------------------- //

        if(pos.get_half_move_clock() == 100) { // 50 move rule
            score = Utils::DRAW_SCORE;
        }

        if(score > best_score) {
            best_score = score;
            if(ply == 0) { // if we have got the score for the best move at root (the one we will give to UCI to play)
                root_best_move = move;
                root_best_score = score;
            }
        }
    }
    if(!has_found_a_legal_move) { // if there are no legal moves at a depth past 1, then 
        if(pos.in_check()) {
            best_score = Utils::MATE_SCORE + ply; // checkmate
        }
        else {
            best_score = Utils::DRAW_SCORE; // stalemate
        }
    }
    return best_score;
}

int Search::iterative_deepening(const Position& pos, Timer& timer) {
    Move last_best_move;
    int last_best_score = -INT_MAX;
    int depth = 1;
    while(true) {
        int score = -negamax(depth, 0, pos, timer);
        if(timer.is_out_of_time()) {
            root_best_move = last_best_move;
            return last_best_score;
        }
        last_best_move = root_best_move;
        last_best_score = root_best_score;
        has_found_a_legal_move = false;
        std::cout << "info score cp " << score << " depth " << depth << std::endl;
        depth++;
    }
    return 0; // shouldnt need to return anything as this shouldnt be reached
}

bool Search::is_repetition(const int& ply, const Position& pos) {
    bool has_repeated = false;
    bool repetition = false;
    const int head = static_cast<int>(zobrist_key_stack.size()) - 1;
    const int max_dist = std::min(static_cast<int>(pos.get_half_move_clock()), head);

// starting from 4 because you cannot have the same position from ply 2 as you cannot loop back in 1 move
    for(int ply_from_current_pos = 4; ply_from_current_pos <= max_dist; ply_from_current_pos += 2) {
        if(pos.get_zobrist_key() == zobrist_key_stack[head - ply_from_current_pos]) {
            if(ply >= ply_from_current_pos) { // if there is a repetition within our search tree, we return true instantly as you only want to check for 2 fold in the search tree
                return true;
            }
            if(has_repeated) { // if has repetition in game history for a second time, return true
                return true;
            }
            has_repeated = true;
            ply_from_current_pos += 2; // same logic as starting from 4, we add an extra two as n - 2 cannot be a repeat
        }
    }
    return false;
}

// int Search::quintescence_search(std::vector<Move>& moves, u64 board) {
//     for(Move move : moves) {
//         if(!Utils::piece_on_square(move.get_dest_square(), board)) { // if not a capture
//             continue;
//         }
//     }
// }