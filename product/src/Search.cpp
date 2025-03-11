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
        //return gambit_search(pos, timer, tt, ps, opp);
        return promise_score_iterative_deepening(pos, timer, tt, ps);
    }
    else {
        return iterative_deepening(pos, timer, tt, ps);
    }
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

// Inspired by: https://www.chessprogramming.org/Alpha-Beta#Negamax_Framework
int Search::alpha_beta(int depth, int ply, Position& pos, Timer& timer, int alpha, int beta, Transposition_Table& tt, PositionStack& ps) {
    // if resulting position after a move is a draw, return draw score instantly
    if(ply > 0) {
        if(is_draw(pos, ps)) { return Utils::DRAW_SCORE; }
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
 * @brief Checks for insufficient mating material, 50 move rule, then repetition
 * 
 * @param ply number of half moves from current search tree depth to root
 * @param pos current position after make move
 * @return true if 50 move or repetition rule has occured
 * @return false if 50 move or repetition rule has not occured
 */
bool Search::is_draw(Position& pos, PositionStack& ps) {
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
    //bool repetition_has_occured = false;
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
        if(is_draw(pos, ps)) { return Utils::DRAW_SCORE; }
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
        score = score * opp.calculate_likelihood(pos, move, score); // calculating weighted score
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

bool compare_by_eval(EvaluatedMove move1, EvaluatedMove move2)
{
    return move1.eval > move2.eval;
}

//stores for the initial pos hence 20 evaluated moves.


/**
 * @brief Applies soft minimax to get promise scores for eval.
 * 
 * @param pos 
 * @param timer 
 * @param tt 
 * @param ps 
 * @param previous_promise_score 
 * @return int 
 */
int Search::promise_score_iterative_deepening(Position& pos, Timer& timer, Transposition_Table& tt, PositionStack& ps)
{
    // Using a vector of promise scores for now to get an idea of how the eval is for each line
    std::vector<Move> moves = pos.generate_all_moves();
    std::vector<EvaluatedMove> evaluated_moves;
    evaluated_moves.reserve(moves.size()); // for example, 20 moves at the start pos
    Move last_best_move;
    int last_best_score = -INT_MAX;

    u64 nodes_evaluated = 0;
    long long promise_score = 0; // can be negative

    for(int depth = 1; depth < 255; depth++)
    {
        for(int i = 0; i < moves.size(); ++i)
        {
            // for each move, we need to count EVERY evaluation performed.
            // pretty sure this would just be during Evaluation::evaluate(), not in retrieval of score.
            // at like depth 4, it would be looking at 20 moves roughly 20 times per depth remaining
            // sp depth 4 could easily see 20^5 steps.
            // do i need a double?

            // will store illegal moves but they should be at the bottom. Perhaps can sort this out with an if somewhere.
            // sorta like generating pseudo legal, then checking later
            // lower level of complexity this way
            // we also flip one last time
            promise_score_search(depth, 1, pos, timer, tt, ps, promise_score, nodes_evaluated);
            evaluated_moves[i] = EvaluatedMove(moves[i], (-promise_score * (int)moves.size()) / (long)(long)nodes_evaluated);

            // Let search finish then grab last depths best move.
            // Soft timeout
            if(timer.is_out_of_time())
            {
                root_best_move = last_best_move; // disregard an incomplete search
                return last_best_score;
            }

        }
        std::sort(evaluated_moves.begin(), evaluated_moves.end(), compare_by_eval);
        // always trust higher depths, so take most recent best move as the best, regardless of the eval of the last best.
        last_best_move = evaluated_moves[0].move;
        last_best_score = evaluated_moves[0].eval;
        std::cout << "info score cp " << last_best_score << " depth " << depth << std::endl;
        // Reset
        has_found_a_legal_move = false;
        promise_score = 0;
        nodes_evaluated = 0;
        evaluated_moves.clear(); // maintains reserved memory


        // at some point i need to average the eval for each move at each depth
    }
    return 0; // should never be reached
}

void Search::promise_score_search(int depth, int ply, Position& pos, Timer& timer, Transposition_Table& tt, PositionStack& ps, long long& promise_score, u64& nodes_evaluated)
{
    // draw
    if(depth == 0)
    {
        // promise score is incremented here, therefore we should also incrememnt num of evals for our average
        nodes_evaluated++;
        promise_score += Evaluation::evaluate(pos);
        return;
    }
    
    int score = -INT_MAX;
    int num_of_moves = 0;
    std::vector<Move> moves = pos.generate_all_moves();
    for(Move move : moves)
    {
        if(timer.is_out_of_time()) return; 

        Position new_pos = pos;
        new_pos.make_move(move);
        if(!new_pos.is_legal(move))
        {
            continue;
        }
        promise_score_search(depth - 1, ply + 1, new_pos, timer, tt, ps, promise_score, nodes_evaluated);
    }

}