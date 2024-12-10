#ifndef UCI_HPP
#define UCI_HPP

#include "Types.hpp"
#include "Search.hpp"
#include "Position.hpp"
#include "Timer.hpp"

#include <chrono>

namespace UCI {
    std::vector<std::string> split_args(std::string input);
    void go(std::vector<std::string>& args, Timer& timer, Position& pos, Transposition_Table& tt, PositionStack& ps);
    void position(std::vector<std::string>& args, Position& pos, PositionStack& ps);
    int options(std::vector<std::string>& args);
}

#endif // #ifndef UCI_HPP