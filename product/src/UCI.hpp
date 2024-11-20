#ifndef UCI_HPP
#define UCI_HPP

#include <chrono>

#include "Types.hpp"
#include "Position.hpp"
#include "Timer.hpp"

namespace UCI {
    std::vector<std::string> split_args(std::string input);
    void go(std::vector<std::string>& args, Timer& timer, Position& pos);
    void position(std::vector<std::string>& args, Position& pos);
}

#endif // #ifndef UCI_HPP