#ifndef UCI_HPP
#define UCI_HPP

#include "Types.hpp"
#include "Search.hpp"
#include "Position.hpp"
#include "Timer.hpp"

#include <chrono>

namespace UCI {
    std::vector<std::string> split_args(std::string input);
    u64 go(std::vector<std::string>& args, Timer& timer, Position& pos, std::vector<u64> game_history_stack, Transposition_Table& tt);
    void position(std::vector<std::string>& args, Position& pos);
    int options(std::vector<std::string>& args);
}

#endif // #ifndef UCI_HPP