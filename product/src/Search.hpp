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

        // Utils
        bool is_repetition(const int& ply, const Position& pos);

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