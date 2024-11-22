#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "utils.hpp"
#include "Position.hpp"

namespace Search {
    Move find_random_move(Position& pos);
    Move find_best_move(Position& pos, Timer& timer, bool use_mvv_lva);
    int negamax(Position& pos, u8 depth, PV* pline, Timer& timer, bool& use_mvv_lva);
    int negamax_ab(Position& pos, u8 depth, int alpha, int beta, PV* pline, Timer& timer);
}

#endif // #ifndef SEARCH_HPP