#ifndef UCI_HPP
#define UCI_HPP

#include <chrono>

#include "Types.hpp"
#include "Position.hpp"
#include "Timer.hpp"

namespace UCI {
    std::vector<std::string> split_args(std::string input);
    Move go(std::vector<std::string>& args, Timer& timer, Position& pos, const Move last_6_half_moves[6]);
    void position(std::vector<std::string>& args, Position& pos);
}

#endif // #ifndef UCI_HPP