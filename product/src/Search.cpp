#include "Types.hpp" // Needed in every file with an assert for assert checker macro

#include "Search.hpp"
#include "Evaluation.hpp"

#include <random>

Search::Search(){}

Search::Search(Search_Type search_type, Search_Algorithm search_algorithm) {
    this->search_type = search_type;
    this->search_algorithm = search_algorithm;
}

Move Search::find_random_move(Position& pos) {
    std::vector<Move> moves = pos.generate_all_moves();
     std::random_device rd;
    // Initialize a Mersenne Twister pseudo-random number generator
    std::mt19937_64 gen(rd());
    // Define the range of random numbers (inclusive)
    std::uniform_int_distribution<size_t> dis(0, moves.size() - 1);
    while(true) {
        Position new_position = pos; // reset position after every exploration
        // make move
        Move move = moves[dis(gen)];
        new_position.make_move(move); // apply move to current pos

        if(new_position.legality_check(move)) { // if move is legal, return move
            std::cout << "info score cp " << static_cast<int>(Evaluation::evaluate(new_position)) << " depth 1\n";
            return move;
        }
    }
}

int Search::negamax2(int depth, int ply, const Position& pos) {
    if(depth == 0) {
        return Evaluation::evaluate(pos);
    }
    int best_score = -INT_MAX;
    std::vector<Move> moves = pos.generate_all_moves();
    for(Move move : moves) {
        // reset position
        Position new_position = pos;
        new_position.make_move(move);
        if(!new_position.legality_check(move)) {
            continue;
        }
        int score = -negamax2(depth - 1, ply + 1, new_position);
        if(score > best_score) {
            best_score = score;
            if(ply == 0) { // if we have got the score for the best move at root (the one we will give to UCI to play)
                root_best_move = move;
            }
        }
    }
    if(!has_found_a_legal_root_move) { // if there are no legal moves at a depth past 1, then 
        best_score = Utils::MATE_SCORE;
    }
    return best_score;
}

// Finds the best move for the current position using negamax
// Initial call for negamax for each move up to a line depth provided
Move Search::find_best_move(Position& pos, Timer& timer, bool use_mvv_lva) {
    int best_score = -INT_MAX;
    Move best_move = Move(0, 0, Move_Flag::NULL_FLAG);
    int score = 0;// increments depth of search until runs out of time
    PV principal_variation; // used to keep track of the best line
    int depth = 1;
    while(true) {
        std::vector<Move> moves = pos.generate_all_moves();
        if(use_mvv_lva) {
            moves = Utils::sort_by_mvv_lva(moves, pos);
        }
        for(Move move : moves) {
            if(timer.is_out_of_time()) {
                return best_move;
            }
            // Reset position
            Position new_position = pos; // reset position after every exploration
            // make move
            new_position.make_move(move); // apply move to current pos

            if(!new_position.legality_check(move)) { // if move isnt legal, skip eval for this move
                continue;
            }
            PV line;

            //**  Negamax: **//
            score = -negamax(new_position, depth - 1, &line, timer, use_mvv_lva);

            //** AB Negamax: **//
            //score = -new_position.negamax_ab(depth - 1, -INT_MAX, INT_MAX, &line, timer);

            if(score > best_score) {
                best_score = score;
                best_move = move;
                //Update the principal variation
                principal_variation.moves.clear();
                principal_variation.moves.push_back(move);
                principal_variation.moves.insert(
                principal_variation.moves.end(), line.moves.begin(), line.moves.end());
                principal_variation.num_of_moves = static_cast<int>(principal_variation.moves.size());
            }
        }
        std::cout << "info score cp " << best_score << " depth " << depth << " pv ";
        for (Move& pv_move : principal_variation.moves) {
            std::cout << Utils::move_to_board_notation(pv_move) << " ";  // Print the principal variation
        }
        std::cout << std::endl;
        depth++;
    }
}

int Search::negamax(Position& pos, u8 depth, PV* pline, Timer& timer, bool& use_mvv_lva) {
    Position new_position = pos;
    if(depth == 0 || timer.is_out_of_time()) {
        pline->num_of_moves = 0;
        return Evaluation::evaluate(new_position);
    }
    PV line;
    int best_score = -INT_MAX;
    std::vector<Move> moves = pos.generate_all_moves();
    if(use_mvv_lva) {
        moves = Utils::sort_by_mvv_lva(moves, pos);
    }
    for(Move move : moves) {
        // Reset position
        new_position = pos; // reset position after every exploration
        
        // make move
        new_position.make_move(move); // apply move to current pos
        
        if(!new_position.legality_check(move)) { // if move isnt legal, skip eval for this move
            continue;
        }

        int score = -negamax(new_position, depth - 1, &line, timer, use_mvv_lva);

        if(score > best_score) {
            best_score = score;
            pline->moves.clear();                 // Clear the previous PV
            pline->moves.push_back(move);         // Add the current move
            pline->moves.insert(pline->moves.end(), line.moves.begin(), line.moves.end());  // Append deeper PV
            pline->num_of_moves = static_cast<int>(pline->moves.size());  // Update PV size
        }
    }
    return best_score;
}

// Inspired from the chess programming wiki (standard convention)
// Search algorithm which produces the same output as minimax but is simpler to implement
// Pruning is less helpful for gambit as it aims to find risky moves, therefore pruning could prove bad
// Added pruning simply but may take it off for the above reason
// negamax_ab returns the same best move, therefore chose to implement this version.
int Search::negamax_ab(Position& pos, u8 depth, int alpha, int beta, PV* pline, Timer& timer) {
    Position new_position = pos;
    
    if (depth == 0 || timer.is_out_of_time()) { // or if checkmate?
        // returns eval of the position at a given depth
        //std::cout << evaluate(current_position);
        pline->num_of_moves = 0;
        return Evaluation::evaluate(new_position);
    }

    PV line;

    int best_score = -INT_MAX;
    std::vector<Move> moves = pos.generate_all_moves();
    for(Move move : moves) {
        // Reset position
        new_position = pos; // reset position after every exploration
        
        // make move
        new_position.make_move(move); // apply move to current pos
        
        if(!new_position.legality_check(move)) { // if move isnt legal, skip eval for this move
            continue;
        }

        int score = -negamax_ab(new_position, depth - 1, -beta, -alpha, &line, timer);

        if(score > best_score) {
            best_score = score;
            if(score > alpha) {
                alpha = score;
                pline->moves.clear();                 // Clear the previous PV
                pline->moves.push_back(move);         // Add the current move
                pline->moves.insert(pline->moves.end(), line.moves.begin(), line.moves.end());  // Append deeper PV
                pline->num_of_moves = static_cast<int>(pline->moves.size());  // Update PV size
            }
        }
        if(score >= beta) {
            return best_score;
        }
    }
    return best_score;
}

// int Search::quintescence_search(std::vector<Move>& moves, u64 board) {
//     for(Move move : moves) {
//         if(!Utils::piece_on_square(move.get_dest_square(), board)) { // if not a capture
//             continue;
//         }
//     }
// }