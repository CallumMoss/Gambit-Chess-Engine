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
        Search(Search_Type search_type, Search_Algorithm search_algorithm);

        Move find_random_move(Position& pos);
        Move find_best_move(Position& pos, Timer& timer, bool use_mvv_lva);
        Move find_best_move_fixed_depth(Timer& timer, int& desired_depth, bool& use_mvv_lva, bool& find_pv);
        int negamax(Position& pos, u8 depth, PV* pline, Timer& timer, bool& use_mvv_lva);
        int negamax_ab(Position& pos, u8 depth, int alpha, int beta, PV* pline, Timer& timer);

        int negamax2(int depth, int ply, const Position& pos);
        int iterative_deepening(const Position& pos, Timer& timer);
        int negamax2_timer(int depth, int ply, const Position& pos, Timer& timer);

        // Getters and Setters
        Move get_root_best_move() { return root_best_move; }
        bool get_has_found_a_legal_move() { return has_found_a_legal_move; }

    private:
        Move root_best_move;
        int root_best_score = -INT_MAX;
        bool has_found_a_legal_move = false;
        Search_Type search_type;
        Search_Algorithm search_algorithm;
};

#endif // #ifndef SEARCH_HPP