#ifndef TIMER_HPP
#define TIMER_HPP

#include "Types.hpp"
#include <chrono>

class Timer {
    public:
        void set_fields(u64 time, u64 increment) {
            players_time = time;
            players_increment = increment;
        }
        void start_timer() {
            start_time = std::chrono::steady_clock::now();
            end_time = start_time + std::chrono::milliseconds(get_time_allowance());
        }
        u64 compute_turn_duration() {
            return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        }
        u64 get_time_allowance() { // amount of time allowed to spend searching this turn
            return u64(u64(players_time / 20) + u64(players_increment / 2));
        }
        bool is_out_of_time() {
            return std::chrono::steady_clock::now() > end_time;
        }

    private:
        u64 players_time;
        u64 players_increment;
        std::chrono::time_point<std::chrono::steady_clock> start_time; // value of the time left at start of turn
        std::chrono::time_point<std::chrono::steady_clock> end_time; // value of the time left at end of turn
};

#endif // #ifndef TIMER_HPP