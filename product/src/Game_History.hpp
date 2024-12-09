#ifndef GAME_HISTORY_HPP
#define GAME_HISTORY_HPP

#include "Types.hpp"
#include "utils.hpp"

class Game_History {
    public:
        Game_History();
        void add(u64 key);
        void pop();
        void clear();
        u64 get_element(int i);

    private:
        std::vector<u64> stack;
};

#endif // #ifndef GAME_HISTORY_HPP