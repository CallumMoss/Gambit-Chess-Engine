#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "Types.hpp"
#include "utils.hpp"
#include "Position.hpp"
#include "Evaluation.hpp"

enum Search_Type {
    FIXED_DEPTH,
    ITERATIVE_DEEPENING
};

enum Search_Algorithm {
    NEGAMAX,
    ALPHA_BETA,
    GAMBIT_SEARCH
};

class Search {
    public:
        Search();
        Search(std::vector<u64> game_history_stack);

        // Search algorithms
        int negamax(int depth, int ply, const Position& pos, Timer& timer);
        int iterative_deepening(const Position& pos, Timer& timer);
        int alpha_beta(int depth, int ply, const Position& pos, Timer& timer, int alpha, int beta);

        // Utils
        bool is_draw(const int& ply, const Position& pos);
        bool three_fold_repetition_has_occured(Move last_6_half_moves[6]);
        int find_mvv_lva(Piece& victim_type, Piece& attacker_type);
        int value_of_piece_from_type_and_capture_role(Piece& type, bool is_victim);
        std::vector<Move> sort_by_mvv_lva(const std::vector<Move>& moves, const Position& pos);


        // Getters and Setters
        Move get_root_best_move() { return root_best_move; }
        bool get_has_found_a_legal_move() { return has_found_a_legal_move; }

    private:
        Move root_best_move;
        int root_best_score = -INT_MAX;
        bool has_found_a_legal_move = false;
        
        std::vector<u64> zobrist_key_stack; // should be max size of 100, maybe 200?
};

#endif // #ifndef SEARCH_HPP