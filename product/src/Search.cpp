#include "Search.hpp"

#include <random>
#include <cmath>
#include <algorithm>

Search::Search():
    root_best_move(),
    root_best_score(-INT_MAX),
    has_found_a_legal_move(false) {}

// Inspired by: https://www.chessprogramming.org/Alpha-Beta#Negamax_Framework
int Search::alpha_beta(int depth, int ply, Position& pos, Timer& timer, int alpha, int beta, Transposition_Table& tt, Game_History& gh) {
    // // if resulting position after a move is a draw, return draw score instantly
    // if(ply != 0) {
    //     if(is_draw(pos, gh, ply)) { return Utils::DRAW_SCORE; }
    // }

    if(depth == 0) { return Evaluation::evaluate(pos); }

    Node_Type node_type = Node_Type::UPPER;
    Move zobrist_move = Utils::NULL_MOVE;
    if(ply != 0) {
        // Checking if position is in transposition table
        // Inspired by https://github.com/TiltedDFA/TDFA/blob/main/src/Search.cpp
        if(tt.entry_is_in_tt(pos.get_zobrist_key())) {
            TT_Entry entry = tt.get_entry(pos.get_zobrist_key());
            zobrist_move = entry.best_move;
            if(entry.depth >= depth) { // if entry has been evaluated at the current desired depth, then we can just return, otherwise we should continue search
                if((entry.node_type == Node_Type::EXACT) || (entry.node_type == Node_Type::UPPER && entry.score <= alpha) || (entry.node_type == Node_Type::LOWER && entry.score >= beta)) {
                    return entry.score; // uses fail soft by returning the score regardless of which condition is true
                }
            }
        }
    }
    int best_score = -INT_MAX;
    int score = -INT_MAX;
    std::vector<Move> moves = pos.generate_all_moves();
    moves = sort_by_mvv_lva(moves, pos);
    Move best_move = moves[0];
    if(!zobrist_move.equals(Utils::NULL_MOVE)) {
        moves.insert(moves.begin(), zobrist_move); // look at zobrist best move first
    }
    for(Move move : moves) {
        if(timer.is_out_of_time()) {
            return best_score; // could return anything as this score wont be used
        }
        // reset position
        Position new_position = pos;
        new_position.make_move(move);
        if(!new_position.is_legal(move)) { continue; }

        if(!has_found_a_legal_move) { has_found_a_legal_move = true; }

        //gh.add(new_position.get_zobrist_key());
        score = -alpha_beta(depth - 1, ply + 1, new_position, timer, -beta, -alpha, tt, gh);
        //gh.pop();

        if(score > best_score) {
            best_score = score;
            best_move = move;
            if(score > alpha) {
                node_type = Node_Type::EXACT;
                alpha = score;
            }
            if(ply == 0) { // if we have got the score for the best move at root (the one we will give to UCI to play)
                root_best_move = move;
                root_best_score = score;
            }
        }
        if(score >= beta) {
            node_type = Node_Type::LOWER;
            tt.add_entry(pos.get_zobrist_key(), score, move, depth, node_type);
            break; // fail soft, we exit here to return the correct score based on whether there is a mate or stalemate
        }
    }

    if(!has_found_a_legal_move) { // if there are no legal moves at a depth past 1, then 
        if(pos.in_check()) {
            best_score = Utils::MATE_SCORE + ply;
            forced_flag = Forced_Flag::CHECKMATE;
        }

        else {
            best_score = Utils::DRAW_SCORE;
            forced_flag = Forced_Flag::STALEMATE;
        }
    }
    // unsure:
    tt.add_entry(pos.get_zobrist_key(), alpha, best_move, depth, node_type);
    return best_score;
}

/**
 * @brief Calls search algorithm and iterates its depth until time is up
 * 
 * @param pos root position
 * @param timer used to keep track of turn allowance
 * @param tt used to store previous positions
 * @return int score of best move or 0
 */
int Search::iterative_deepening(Position& pos, Timer& timer, Transposition_Table& tt, Game_History& gh) {
    Move last_best_move;
    int last_best_score = -INT_MAX;
    int depth = 1;
    while(true) {
        int score = -alpha_beta(depth, 0, pos, timer, -INT_MAX, INT_MAX, tt, gh);
        if(forced_flag != Forced_Flag::NO_FORCED) { // if there is a forced position, stop searching further
            last_best_move = root_best_move;
            last_best_score = root_best_score;
            std::cout << "info score cp " << score << " depth " << depth << std::endl;
            return score;
        }

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
bool Search::is_draw(Position& pos, Game_History& gh, int ply) {
    // Check for insufficient mating material
    // Inspired by: https://github.com/zzzzz151/Starzix/blob/main/src/board.hpp
    u64 board = pos.get_board();
    // King vs King
    if (Utils::count_number_of_1bs(board) == 2) { return true; }
    // King and Bishop vs King or King and Knight vs King
    if (Utils::count_number_of_1bs(board) == 3) {
        if(pos.get_knights() > 0 || pos.get_bishops() > 0) {
            return true;
        }
    }

    // 50 move rule:
    if(pos.get_half_move_clock() == 100) { return true; }

    // Repetition: (Checks for 2 fold repetition)
    if(pos.get_half_move_clock() < 4) { return false; } // no repetition could have occured
    int stack_top_index = gh.get_index();
    if(stack_top_index < 4) { return false; } // no repetition could have occured
    bool repetition_has_occured = false;
    for(int i  = 1; i <= stack_top_index; i++) {
        if(pos.get_zobrist_key() == gh.get_element(stack_top_index - i)) {
            if(repetition_has_occured) { return true; } // if repetition has already occured, return true
            repetition_has_occured = true;
        }
    }
    return false;
}

// Most Valuable Victim - Least Valuable Aggressor
// Simple heuristic to help sort captures in a way where it is more likely to benefit from the exchange
// Ranges from [4, 49], where the lower the number the higher the priority
int Search::find_mvv_lva(Piece victim_type, Piece attacker_type) { // we give weight towards the victim to ensure it gets sorted first
    return ((value_of_piece_from_type_and_capture_role(victim_type, true) * 10) - value_of_piece_from_type_and_capture_role(attacker_type, false));
}

int Search::value_of_piece_from_type_and_capture_role(Piece type, bool is_victim) {
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
    std::vector<MVV_LVA_Log> move_logs;
    // Iterate through each move
    for(Move move : moves) {
        // Check if it's a capture (if the destination has a piece)
        if(!Utils::piece_is_at_square(pos.get_board(), static_cast<int>(move.get_dest_square()))) {
            // Not a capture, assign lowest priority (50)
            MVV_LVA_Log move_log = {move, 50};
            move_logs.push_back(move_log);
        }
        else {
            // It's a capture, calculate MVV-LVA score
            Piece victim = pos.get_piece_type_from_square(move.get_dest_square());
            Piece attacker = pos.get_piece_type_from_square(move.get_src_square());
            int mvv_lva_score = find_mvv_lva(victim, attacker); // This should give a score prioritizing valuable captures

            // Log the move and its score
            MVV_LVA_Log move_log = {move, mvv_lva_score};
            move_logs.push_back(move_log);
        }
    }

    // Sort the moves based on MVV-LVA score, higher priority first
    std::stable_sort(move_logs.begin(), move_logs.end(), [](const MVV_LVA_Log& a, const MVV_LVA_Log& b) {
        return a.mvv_lva_score < b.mvv_lva_score; // lower scores at the front
    });

    // Extract sorted moves from the logs
    std::vector<Move> sorted_moves;
    for(MVV_LVA_Log& log : move_logs) {
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