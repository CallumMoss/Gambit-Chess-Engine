#include "Search.hpp"

#include <random>
#include <cmath>
#include <algorithm>

Search::Search(){}

Search::Search(std::vector<u64> game_history_stack) { zobrist_key_stack = game_history_stack; }

int Search::negamax(int depth, int ply, const Position& pos, Timer& timer) {
    if(depth == 0) {
        return Evaluation::evaluate(pos);
    }
    int best_score = -INT_MAX;
    int score = -INT_MAX;
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

        // Checking for 50 move and repetition
        if(is_draw(ply, new_position)) {
            // Score is draw score and search from this node is stopped as a draw wouldve already occured
            score = Utils::DRAW_SCORE;
            std::cout << "Repetition detected at depth: " << depth << std::endl;
            std::cout << Utils::move_to_board_notation(move) << std::endl;
            //std::exit(-1);
            // position startpos moves b1c3 b8c6 c3b1 c6b8 b1c3 b8c6 c3b1 c6b8 b1c3 b8c6 c3b1 c6b8
        }
        else {
            // Continue searching
            score = -negamax(depth - 1, ply + 1, new_position, timer);
            // std::cout << "Not repetition\n";
            // std::cout << Utils::move_to_board_notation(move) << std::endl;
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

// Inspired by: https://www.chessprogramming.org/Alpha-Beta#Negamax_Framework
int Search::alpha_beta(int depth, int ply, const Position& pos, Timer& timer, int alpha, int beta) {
    if(depth == 0) {
        return Evaluation::evaluate(pos);
    }
    int best_score = -INT_MAX;
    int score = -INT_MAX;
    std::vector<Move> moves = pos.generate_all_moves();
    moves = sort_by_mvv_lva(moves, pos);
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

        // Checking for 50 move and repetition
        if(is_draw(ply, new_position)) {
            // Score is draw score and search from this node is stopped as a draw wouldve already occured
            score = Utils::DRAW_SCORE;
            std::cout << "Repetition detected at depth: " << depth << std::endl;
            std::cout << Utils::move_to_board_notation(move) << std::endl;
            //std::exit(-1);
            // position startpos moves b1c3 b8c6 c3b1 c6b8 b1c3 b8c6 c3b1 c6b8 b1c3 b8c6 c3b1 c6b8
        }
        else {
            // Continue searching
            score = -alpha_beta(depth - 1, ply + 1, new_position, timer, -beta, -alpha);
            // std::cout << "Not repetition\n";
            // std::cout << Utils::move_to_board_notation(move) << std::endl;
        }

        if(score > best_score) {
            best_score = score;
            if(score > alpha) {
                alpha = score;
            }
            if(ply == 0) { // if we have got the score for the best move at root (the one we will give to UCI to play)
                root_best_move = move;
                root_best_score = score;
            }
        }
        if(score >= beta) {
            break; // fail soft, we exit here to return the correct score based on whether there is a mate or stalemate
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
        // Negamax //
        //int score = -negamax(depth, 0, pos, timer);
        //         //

        // Alpha Beta //
        int score = -alpha_beta(depth, 0, pos, timer, -INT_MAX, INT_MAX);
        //            //

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

/**
 * @brief Checks for 50 move rule, then repetition
 * 
 * @param ply number of half moves from current search tree depth to root
 * @param pos current position after make move
 * @return true if 50 move or repetition rule has occured
 * @return false if 50 move or repetition rule has not occured
 */
bool Search::is_draw(const int& ply, const Position& pos) {
    // 50 move rule:
    if(pos.get_half_move_clock() == 100) {
        std::cout << "\n\nHello\n\n";
        return true;
    }
    // Repetition:
    bool has_repeated = false;
    bool repetition = false;
    const int head = static_cast<int>(zobrist_key_stack.size()) - 1;
    const int max_dist = std::min(static_cast<int>(pos.get_half_move_clock()), head);

// starting from 4 because you cannot have the same position from ply 2 as you cannot loop back in 1 move
    for(int ply_from_current_pos = 4; ply_from_current_pos <= max_dist; ply_from_current_pos += 2) {
        // std::cout << pos.get_zobrist_key() << std::endl;
        // std::cout << zobrist_key_stack[head - ply_from_current_pos] << std::endl;
        if(pos.get_zobrist_key() == zobrist_key_stack[head - ply_from_current_pos]) {
        //             std::cout << pos.get_zobrist_key() << std::endl;
        // std::cout << zobrist_key_stack[head - ply_from_current_pos] << std::endl;
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

// Most Valuable Victim - Least Valuable Aggressor
// Simple heuristic to help sort captures in a way where it is more likely to benefit from the exchange
// Ranges from [4, 49], where the lower the number the higher the priority
int Search::find_mvv_lva(Piece& victim_type, Piece& attacker_type) { // we give weight towards the victim to ensure it gets sorted first
    return ((value_of_piece_from_type_and_capture_role(victim_type, true) * 10) - value_of_piece_from_type_and_capture_role(attacker_type, false));
}

int Search::value_of_piece_from_type_and_capture_role(Piece& type, bool is_victim) {
    if(is_victim) {
        switch(type) {
            case Piece::PAWN:
                return 5;
            case Piece::KNIGHT:
                return 4;
            case Piece::BISHOP:
                return 3;
            case Piece::ROOK:
                return 2;
            case Piece::QUEEN:
                return 1;
            default: // king cannot be taken
                std::cerr << "Unexpected piece type of victim: " << type;
                std::exit(-1);
        }
    }
    //else if attacker
    return type + 1; // king is most valuable attacker as it implies that it is free, as he cannot walk into check
}

std::vector<Move> Search::sort_by_mvv_lva(const std::vector<Move>& moves, const Position& pos) {
    std::vector<Mvv_lva_log> move_logs;
    // Iterate through each move
    for(Move move : moves) {
        // Check if it's a capture (if the destination has a piece)
        if(!Utils::piece_is_at_square(pos.get_board(), static_cast<int>(move.get_dest_square()))) {
            // Not a capture, assign lowest priority (50)
            Mvv_lva_log move_log = {move, 50};
            move_logs.push_back(move_log);
        } else {
            // It's a capture, calculate MVV-LVA score
            Piece victim = pos.get_piece_type_from_square(move.get_dest_square());
            Piece attacker = pos.get_piece_type_from_square(move.get_src_square());
            int mvv_lva_score = find_mvv_lva(victim, attacker); // This should give a score prioritizing valuable captures

            // Log the move and its score
            Mvv_lva_log move_log = {move, mvv_lva_score};
            move_logs.push_back(move_log);
        }
    }

    // Sort the moves based on MVV-LVA score, higher priority first
    std::sort(move_logs.begin(), move_logs.end(), [](const Mvv_lva_log& a, const Mvv_lva_log& b) {
        return a.mvv_lva_score < b.mvv_lva_score; // lower scores at the front
    });

    // Extract sorted moves from the logs
    std::vector<Move> sorted_moves;
    for(Mvv_lva_log& log : move_logs) {
        sorted_moves.push_back(log.move);
    }
    
    return sorted_moves;
}

/**
 * @brief Determines whether a 3 fold repetition has occured based on the past 6 half moves
 * 
 * @return true if a repetition has occured
 * @return false if a repetition has not occured
 */
bool Search::three_fold_repetition_has_occured(Move last_6_half_moves[6]) {
    Move null_move;
    for(int i = 0; i < 6; i++) { // checking if not equal to the inital null array
        if(last_6_half_moves[i].equals(null_move)) { return false; }    
    }
    if(last_6_half_moves[0].equals(last_6_half_moves[2])) {
        if(last_6_half_moves[2].equals(last_6_half_moves[4])) {
            if(last_6_half_moves[1].equals(last_6_half_moves[3])) {
                if(last_6_half_moves[3].equals(last_6_half_moves[5])) {
                    return true;
                }
            }
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