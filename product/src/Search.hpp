#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "Types.hpp"
#include "utils.hpp"
#include "Position.hpp"
#include "Evaluation.hpp"
#include "Transposition_Table.hpp"
#include "Game_History.hpp"

enum Forced_Flag {
    NO_FORCED,
    STALEMATE,
    CHECKMATE
};

class Search {
    public:
        Search();
        //Search(std::vector<u64>& game_history_stack, int tt_size_in_mb);

        // Search algorithms
        int iterative_deepening(Position& pos, Timer& timer, Transposition_Table& tt, Game_History& gh);
        int alpha_beta(int depth, int ply, Position& pos, Timer& timer, int alpha, int beta, Transposition_Table& tt, Game_History& gh);

        // Utils
        bool is_draw(Position& pos, Game_History& gh, int ply);
        bool three_fold_repetition_has_occured(Move last_6_half_moves[6]);
        int find_mvv_lva(Piece victim_type, Piece attacker_type);
        int value_of_piece_from_type_and_capture_role(Piece type, bool is_victim);
        std::vector<Move> sort_by_mvv_lva(const std::vector<Move>& moves, const Position& pos);


        // Getters and Setters
        Move get_root_best_move() { return root_best_move; }
        bool get_has_found_a_legal_move() { return has_found_a_legal_move; }

    private:
        Move root_best_move;
        int root_best_score = -INT_MAX;
        bool has_found_a_legal_move = false;
        Forced_Flag forced_flag = Forced_Flag::NO_FORCED;
};

#endif // #ifndef SEARCH_HPP