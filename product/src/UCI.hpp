#ifndef UCI_HPP
#define UCI_HPP

#include "Types.hpp"
#include "Search.hpp"
#include "Position.hpp"
#include "Timer.hpp"

#include <chrono>

namespace UCI {
    std::vector<std::string> split_args(std::string input);
    void go(std::vector<std::string>& args, Timer& timer, Position& pos, Transposition_Table& tt, PositionStack& ps, bool is_gambit, Opponent& opp, std::vector<EvaluatedMove>& evaluated_opp_responses);
    void position(std::vector<std::string>& args, Position& pos, PositionStack& ps, bool is_gambit, Opponent& opp, std::vector<EvaluatedMove>& evaluated_opp_responses);
    int options(std::vector<std::string>& args);
}

#endif // #ifndef UCI_HPP