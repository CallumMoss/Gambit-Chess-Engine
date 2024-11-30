#ifndef UCI_HPP
#define UCI_HPP

#include "Types.hpp"
#include "Search.hpp"
#include "Position.hpp"
#include "Timer.hpp"

#include <chrono>

namespace UCI {
    std::vector<std::string> split_args(std::string input);
    u64 go(std::vector<std::string>& args, Timer& timer, Position& pos, std::vector<u64> game_history_stack);
    void position(std::vector<std::string>& args, Position& pos);
}

#endif // #ifndef UCI_HPP