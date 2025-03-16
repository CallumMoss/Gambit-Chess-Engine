#include "Search.hpp"

#include <random>
#include <cmath>
#include <algorithm>

void order_moves(std::vector<Move>& moves, Move tt_move, Position& pos); // forward declaring (as to make this local to Search.cpp)
void order_moves_qsearch(std::vector<Move>& moves, Position& pos);

Search::Search(bool gambit_flag, Opponent& opp):
    root_best_move(),
    root_best_score(-INT_MAX),
    is_gambit(gambit_flag),
    opponent(opp){}

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

    for(int depth = 1; depth < 256; depth++)
    {
        alpha_beta(depth, 0, pos, timer, -INT_MAX, INT_MAX, tt, ps);
        // if score - pos full move or half move number is less than a certain score, should stop search.
        if(timer.is_out_of_time())
        {
            root_best_move = last_best_move;
            return last_best_score;
        }
        last_best_move = root_best_move;
        last_best_score = root_best_score;
        // has_found_a_legal_move = false;
        std::cout << "depth " << depth << " info score cp " << root_best_score << std::endl;
        //std::cout << Utils::move_to_board_notation(root_best_move) << std::endl;
    }
    return 0; // shouldnt need to return anything as this shouldnt be reached
}

// Inspired by: https://www.chessprogramming.org/Alpha-Beta#Negamax_Framework
int Search::alpha_beta(int depth, int ply, Position& pos, Timer& timer, int alpha, int beta, Transposition_Table& tt, PositionStack& ps)
{
    // if resulting position after a move is a draw, return draw score instantly
    if(ply > 0)
    {
        if(is_draw(pos, ps)) { return Utils::DRAW_SCORE; }
    }

    // if(depth == 0) return Evaluation::evaluate(pos);
    if(depth <= 0) { return quiescence_search(pos, 0, alpha, beta, timer, tt); }

    Node_Type node_type = Node_Type::UPPER;
    Move zobrist_move = Utils::NULL_MOVE;

    if(ply > 0)
    {
        // Checking if position is in transposition table
        // Inspired by https://github.com/TiltedDFA/TDFA/blob/main/src/Search.cpp
        if(tt.entry_is_in_tt(pos.get_zobrist_key()))
        {
            TT_Entry entry = tt.get_entry(pos.get_zobrist_key());
            zobrist_move = entry.best_move;
            if(entry.depth >= depth) { // if entry has been evaluated at the current desired depth, then we can just return, otherwise we should continue search
                if((entry.node_type == Node_Type::EXACT) || (entry.node_type == Node_Type::UPPER && entry.score <= alpha) || (entry.node_type == Node_Type::LOWER && entry.score >= beta)) {
                    return entry.score; // uses fail soft by returning the score regardless of which condition is true
                }
            }
        }
    }

    if (depth <= 6 && !pos.in_check())
    {
        int static_eval = Evaluation::evaluate(pos);
        if(static_eval - 80 * depth >= beta)
        {
            return static_eval;
        }
    }

    int best_score = -INT_MAX;
    int score = -INT_MAX;
    bool has_found_a_legal_move = false;
    std::vector<Move> moves = pos.generate_all_moves(false);
    order_moves(moves, zobrist_move, pos);

    Move best_move = moves[0];

    for(Move move : moves)
    {
        if(timer.is_out_of_time())
        {
            return best_score; // could return anything as this score wont be used
        }
        // reset position
        Position new_position = pos;
        new_position.make_move(move);
        if(!new_position.is_legal(move)) { continue; }

        if(!has_found_a_legal_move) { has_found_a_legal_move = true; }

        ps.push_back(pos.get_zobrist_key()); // adds original position, which in the next call is the next position and checks for draw first
        score = -alpha_beta(depth - 1, ply + 1, new_position, timer, -beta, -alpha, tt, ps);
        //if(is_gambit) score *= opponent.calculate_likelihood(pos, move, score); // calculating weighted score
        ps.pop_back();

        if(score > best_score)
        {
            best_score = score;
            best_move = move;
            if(score > alpha)
            {
                node_type = Node_Type::EXACT;
                alpha = score;
            }
            if(ply == 0)
            { // if we have got the score for the best move at root (the one we will give to UCI to play)
                root_best_move = move;
                root_best_score = score;
            }
        }
        if(score >= beta)
        {
            node_type = Node_Type::LOWER;
            tt.add_entry(pos.get_zobrist_key(), score, move, depth, node_type);
            break; // fail soft, we exit here to return the correct score based on whether there is a mate or stalemate
        }
    }

    if(!has_found_a_legal_move)
    { // if there are no legal moves at a depth past 1, then 
        if(pos.in_check())
        {
            //forced_flag = Forced_Flag::CHECKMATE;
            best_score = Utils::MATE_SCORE + ply; // An earlier checkmate is better than a later one
        }
        else { best_score = Utils::DRAW_SCORE; } // All draws are the same regardless of when they are
        return best_score;
    }
    tt.add_entry(pos.get_zobrist_key(), best_score, best_move, depth, node_type);
    return best_score;
}

int Search::quiescence_search(Position& pos, int ply, int alpha, int beta, Timer& timer, Transposition_Table& tt)
{
    Move zobrist_move = Utils::NULL_MOVE;

    if(ply > 0)
    {
        // Checking if position is in transposition table
        // Inspired by https://github.com/TiltedDFA/TDFA/blob/main/src/Search.cpp
        if(tt.entry_is_in_tt(pos.get_zobrist_key()))
        {
            TT_Entry entry = tt.get_entry(pos.get_zobrist_key());
            zobrist_move = entry.best_move;
            if((entry.node_type == Node_Type::EXACT) || (entry.node_type == Node_Type::UPPER && entry.score <= alpha) || (entry.node_type == Node_Type::LOWER && entry.score >= beta)) {
                return entry.score; // uses fail soft by returning the score regardless of which condition is true
            }
        }
    }

    int stand_pat = Evaluation::evaluate(pos);
    int best_score = stand_pat;
    int score = -INT_MAX;

    if(stand_pat >= beta) { return stand_pat; }
    if (alpha < stand_pat) { alpha = stand_pat; }

    std::vector<Move> noisy_moves = pos.generate_all_moves(true); // moves involving captures
    // mvv lva just makes pruning more frequent, therefore search speeds up.
    // mvv_lva here is necessary as there is no cap to depth like in negamax
    // it will search to crazy depths on bad sequences.
    order_moves(noisy_moves, zobrist_move, pos);

    for(Move& move : noisy_moves)
    {
        // if(timer.is_out_of_time())
        // {
        //     return best_score; // could return anything as this score wont be used
        // }
        Position new_position = pos;
        new_position.make_move(move);
        if(!new_position.is_legal(move)) { continue; }

        score = -quiescence_search(new_position, ply + 1, -beta, -alpha, timer, tt);

        if(score >= beta) { return score; }
        if(score > best_score) { best_score = score; }
        if(score > alpha) { alpha = score; }   
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
bool Search::is_draw(Position& pos, PositionStack& ps) // stalemate is checked elsewhere
{
    // Check for insufficient mating material
    // Inspired by: https://github.com/zzzzz151/Starzix/blob/main/src/board.hpp
    u64 board = pos.get_board();
    // King vs King
    if (Utils::count_number_of_1bs(board) == 2) { return true; }
    // King and Bishop vs King or King and Knight vs King
    if (Utils::count_number_of_1bs(board) == 3)
    {
        if(pos.get_knights() > 0 || pos.get_bishops() > 0)
        {
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
    for(int i = 3; i <= search_cap; i++)
    {
        if(pos.get_zobrist_key() == ps[stack_top_index - i])
        {
            return true;
            // if(repetition_has_occured) { return true; } // if repetition has already occured, return true
            // repetition_has_occured = true;
            //pos.print_position();
        }
    }
    return false;
}

int calc_mvv_lva(Piece victim_type, Piece attacker_type)
{
    if(victim_type == Piece::INVALID || attacker_type == Piece::INVALID)
    {
        return 4; // lowest score
    }
    return ((victim_type + 1) * 10) - attacker_type;
}

void order_moves(std::vector<Move>& moves, Move tt_move, Position& pos)
{
    std::sort(moves.begin(), moves.end(), [&](Move &a, Move &b) {
        u8 score_a = a.equals(tt_move)
            ? 51
            : calc_mvv_lva(
                  pos.get_piece_type_from_square(a.get_dest_square()),
                  pos.get_piece_type_from_square(a.get_src_square())
              );
        u8 score_b = b.equals(tt_move)
            ? 51
            : calc_mvv_lva(
                  pos.get_piece_type_from_square(b.get_dest_square()),
                  pos.get_piece_type_from_square(b.get_src_square())
              );
        return score_a > score_b;
    });
}