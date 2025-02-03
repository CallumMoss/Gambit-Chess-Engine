#include "Search.hpp"

#include <random>
#include <cmath>
#include <algorithm>

Search::Search(bool gambit_flag):
    root_best_move(),
    root_best_score(-INT_MAX),
    has_found_a_legal_move(false), 
    is_gambit(gambit_flag) {}


int Search::search(Position& pos, Timer& timer, Transposition_Table& tt, PositionStack& ps, Opponent& opp) {
    if(is_gambit) {
        return gambit_search(pos, timer, tt, ps, opp);
    }
    else {
        return iterative_deepening(pos, timer, tt, ps);
    }
}

// Inspired by: https://www.chessprogramming.org/Alpha-Beta#Negamax_Framework
int Search::alpha_beta(int depth, int ply, Position& pos, Timer& timer, int alpha, int beta, Transposition_Table& tt, PositionStack& ps) {
    // if resulting position after a move is a draw, return draw score instantly
    if(ply > 0) {
        if(is_draw(pos, ps, ply)) { return Utils::DRAW_SCORE; }
    }

    if(depth == 0) { return Evaluation::evaluate(pos); }

    Node_Type node_type = Node_Type::UPPER;
    Move zobrist_move = Utils::NULL_MOVE;
    if(ply > 0) {
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
    //moves = sort_by_mvv_lva(moves, pos);
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

        ps.push_back(pos.get_zobrist_key()); // adds original position, which in the next call is the next position and checks for draw first
        score = -alpha_beta(depth - 1, ply + 1, new_position, timer, -beta, -alpha, tt, ps);
        ps.pop_back();

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
        if(pos.in_check()) { best_score = Utils::MATE_SCORE + ply; }
        else { best_score = Utils::DRAW_SCORE; }
        return best_score;
    }
    tt.add_entry(pos.get_zobrist_key(), alpha, best_move, depth, node_type);
    return best_score;
}

int Search::quiescence_search(Position& pos, int alpha, int beta) {
    int stand_pat = Evaluation::evaluate(pos);
    int best_score = stand_pat;
    int score = -INT_MAX;
    if(stand_pat >= beta) { return stand_pat; }
    if (alpha < stand_pat) { alpha = stand_pat; }

    std::vector<Move> moves = pos.generate_all_moves();
    for(Move& move : moves) {
        Position new_position = pos;
        new_position.make_move(move);
        if(!new_position.is_legal(move)) { continue; }

        //if(!has_found_a_legal_move) { has_found_a_legal_move = true; }

        //ps.push_back(pos.get_zobrist_key()); // adds original position, which in the next call is the next position and checks for draw first
        score = -quiescence_search(pos, -beta, -alpha);
        //ps.pop_back();

        if(move.is_noisy(pos.get_board())) {
            if(score >= beta) { return score; }
            if(score > best_score) { best_score = score; }
            if(score > alpha) { alpha = score; }   
        }
    }
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
int Search::iterative_deepening(Position& pos, Timer& timer, Transposition_Table& tt, PositionStack& ps) {
    Move last_best_move;
    int last_best_score = -INT_MAX;
    int depth = 1;
    while(depth <= 255) {
        int score = -alpha_beta(depth, 0, pos, timer, -INT_MAX, INT_MAX, tt, ps);
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
 * @brief Checks for insufficient mating material, 50 move rule, then repetition
 * 
 * @param ply number of half moves from current search tree depth to root
 * @param pos current position after make move
 * @return true if 50 move or repetition rule has occured
 * @return false if 50 move or repetition rule has not occured
 */
bool Search::is_draw(Position& pos, PositionStack& ps, int ply) {
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
    int stack_top_index = ps.size() - 1;
    bool repetition_has_occured = false;
    int search_cap = stack_top_index;
    //int search_cap = std::min(stack_top_index, static_cast<int>(pos.get_half_move_clock()));
    for(int i = 3; i <= search_cap; i++) {
        if(pos.get_zobrist_key() == ps[stack_top_index - i]) {
            return true;
            // if(repetition_has_occured) { return true; } // if repetition has already occured, return true
            // repetition_has_occured = true;
            //pos.print_position();
        }
    }
    return false;
}

// Most Valuable Victim - Least Valuable Aggressor
// Simple heuristic to help sort captures in a way where it is more likely to benefit from the exchange
// Ranges from [4, 49], where the lower the number the higher the priority
int Search::find_mvv_lva(Piece victim_type, Piece attacker_type)
{ // we give weight towards the victim to ensure it gets sorted first
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

bool compare_by_eval(EvaluatedMove move1, EvaluatedMove move2)
{
    return move1.eval < move2.eval;
}


// void Search::approach1_initial_call(Position& pos) { // fixed depth implementation for now
//     int depth = 2;
//     Opponent model = Opponent();
//     // int AB = -alpha_beta();
//     // for each move from our POV, find the eval of the opponents response, keep these in a list and sort to get highest and lowest
//     // AB would be somewhere at ply 1, then at ply 2 should be anywhere too?
//     // AB is the move which results in the best eval provided the opponent picks the best move.
//     // A list of lists, where for each list (option for machine), log each eval and move black could respond with.
//     // AB would assume that black picks the lowest in the list, which makes the total eval of the list for the machine that eval.

//     // Step 1: Get all moves and their evals into a data structure
//     std::vector<Move> moves = pos.generate_all_moves();

//     // An array of arrays, which store the evaluated moves after the opponent has responded (depth 2)
//     // There will be n arrays, where n is the number of possible moves for the engine at the startpos (for example 20)
//     // There will be m elements in each array, where m can vary and is the number of moves the opponent can response with.
//     // Should confirm this with perft
//     std::vector<std::vector<EvaluatedMove>> evaluated_moves;
//     for(Move move : moves) {
//         Position new_position = pos;
//         new_position.make_move(move);
//         if(!new_position.is_legal(move)) { continue; }
//         std::vector<Move> response_moves = new_position.generate_all_moves();
//         std::vector<EvaluatedMove> response_for_each_move;
//         for(Move response_move : response_moves) {
//             Position response_position = new_position;
//             response_position.make_move(response_move);
//             if(!response_position.is_legal(move)) { continue; }
//             response_for_each_move.push_back(EvaluatedMove(move, -Evaluation::evaluate(new_position)));
//         }
//         evaluated_moves.push_back(response_for_each_move);
//     }

//     // Step 2: Confirm that we can agree on the AB move.
//     // AB move should be the one that is the highest eval out of the lowest evaled response moves
//     // because eval is flipped, what happens then?
//     // For now assume engine is white
//     // Assume eval does white - black
//     // Sort response moves by eval
//     // Create a new list and sort moves out of the lowest evaled in response
//     std::vector<EvaluatedMove> moves_to_consider;
//     for(std::vector<EvaluatedMove> evaluated_response_moves : evaluated_moves) {
//         EvaluatedMove opponent_best_move = evaluated_response_moves[0];
//         for(EvaluatedMove evaluated_response_move : evaluated_response_moves) {
//             // black should choose the lowest eval here according to alpha beta
//             if(evaluated_response_move.eval < opponent_best_move.eval) {
//                 opponent_best_move = evaluated_response_move;
//             }
//         }
//         moves_to_consider.push_back(opponent_best_move);
//     }

//     EvaluatedMove best_move = moves_to_consider[0];
//     for(EvaluatedMove move : moves_to_consider) {
//         if(move.eval < -best_move.eval) {
//             best_move = move;
//         }
//     }
//     EvaluatedMove alpha_beta_move = best_move;

//     // Step 3:
//     // Now that we know how the AB works, we can begin to work on Gambit.
//     // Instead of only choosing from the best of a bad bunch, lets use the AB move as a baseline.
//     // We can instantly ignore any moves whos success eval is lower than AB eval, as AB is 100% guranteed to be at least this if not higher.

//     // Sort response moves in terms of eval, iterate through these moves and break when eval is less than AB eval.

//     std::vector<EvaluatedMove> better_moves;

//     for(std::vector<EvaluatedMove> evaluated_response_moves : evaluated_moves) {
//         for(EvaluatedMove response_move : evaluated_response_moves) {
//             if(response_move.eval < alpha_beta_move.eval) {
//                 better_moves.push_back(response_move);
//             }
//         }
//     }
//     std::sort(better_moves.begin(), better_moves.end(), compare_by_eval); // explore most promising moves first
//     // compare risk to reward to AB move
//     std::vector<EvaluatedMove> promising_moves;
//     for(EvaluatedMove move : better_moves) {
//         if(model.should_play0(alpha_beta_move.eval, move.eval)) {
//             promising_moves.push_back(move);
//         }
//     }
//     std::sort(promising_moves.begin(), promising_moves.end(), compare_by_eval); // explore most promising moves first
//     root_best_move = promising_moves[0].move;
//     // std::cout << promising_moves.size() << std::endl;
//     for(EvaluatedMove move : promising_moves) {
//         std::cout << Utils::move_to_board_notation(move.move) << std::endl;
//         std::cout << move.eval << std::endl;
//     }
//     std::cout << Utils::move_to_board_notation(promising_moves[0].move) << std::endl;
//     std::cout << promising_moves[0].eval << std::endl;

//     int top = ((int)promising_moves.size()) - 1;
//     std::cout << Utils::move_to_board_notation(promising_moves[top].move) << std::endl;
//     std::cout << promising_moves[top].eval << std::endl;
// }

void Search::approach0_initial_call(Position& pos)
{
    int max_depth = 2;
    // Get all moves for each starting move
    std::vector<std::vector<EvaluatedMove>> evaluated_moves;
    evaluated_moves.reserve(Utils::MAX_PLY); // reserving up to 255 depth, 218 being the max number of moves in a chess position.
    // instead of storing all here, we will instead store all moves from one move at ply 0, then eval it, clear it and do it for the next move
    for(int depth = 1; depth <= max_depth; ++depth)
    {
        int score = -approach0(depth, 0, pos, evaluated_moves);
        std::cout << "info score cp " << score << " depth " << depth << std::endl;
    }
}

// Index is the index of the possible moves at the startpos
int Search::approach0(int depth, int ply, Position& pos, std::vector<std::vector<EvaluatedMove>>& evaluated_moves)
{
    if(depth == 0) { return Evaluation::evaluate(pos); }
    int best_score = -INT_MAX;
    int score = -INT_MAX;
    std::vector<Move> moves = pos.generate_all_moves();
    for(Move move : moves)
    {
        Position new_position = pos;
        new_position.make_move(move);
        if(!new_position.is_legal(move)) { continue; }
        score = approach0(depth - 1, ply + 1, new_position, evaluated_moves);
        evaluated_moves[ply].push_back(EvaluatedMove(move, score));
    }
    return score;
}

/**
 * @brief Acts as iterative deepening with modified behaviours
 * 
 * @return int 
 */
int Search::gambit_search(Position& pos, Timer& timer, Transposition_Table& tt, PositionStack& ps, Opponent& opp) {
    Move last_best_move;
    int last_best_score = -INT_MAX;
    int depth = 1;
    while(depth <= 255) {
        int score = -alpha_beta_prediction(depth, 0, pos, timer, -INT_MAX, INT_MAX, tt, ps, opp);
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
 * @brief Same as alpha beta but evaluates with probability too
 * 
 * @return int 
 */
int Search::alpha_beta_prediction(int depth, int ply, Position& pos, Timer& timer, int alpha, int beta, Transposition_Table& tt, PositionStack& ps, Opponent& opp) {
    // if resulting position after a move is a draw, return draw score instantly
    if(ply > 0) {
        if(is_draw(pos, ps, ply)) { return Utils::DRAW_SCORE; }
    }

    if(depth == 0) { return Evaluation::evaluate(pos); }

    Node_Type node_type = Node_Type::UPPER;
    Move zobrist_move = Utils::NULL_MOVE;
    if(ply > 0) {
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
    //moves = sort_by_mvv_lva(moves, pos);
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

        ps.push_back(pos.get_zobrist_key()); // adds original position, which in the next call is the next position and checks for draw first
        score = -alpha_beta(depth - 1, ply + 1, new_position, timer, -beta, -alpha, tt, ps);
        score = score * opp.calculate_likelihood(pos, move); // calculating weighted score
        ps.pop_back();

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
        if(pos.in_check()) { best_score = Utils::MATE_SCORE + ply; }
        else { best_score = Utils::DRAW_SCORE; }
        return best_score;
    }
    tt.add_entry(pos.get_zobrist_key(), alpha, best_move, depth, node_type);
    return best_score;
}

// We need to use minmax instead of negamax, as engine chooses to "maximise", but opponent doesnt minimise
// We can still use alpha beta, if we just assume that the opponents goal isnt trying to minimise the score
// Of course, the opponent will be, but we can play with the opposite for now
// Return score rather than -score?
// Its funny, im making my engine purposely play the worse moves (for the opponent), so that I can learn how humans play
// In a way, search assumes the opponents goal isnt to find the best move, but one thats kind of good
// At each ply, if opponent, store all scores? Maybe just store all scores within our desired score based on probability (like in the report)
// instead of if score > best_score: best_move = move, we do if gambit_calc(score, best_score): best_move = move