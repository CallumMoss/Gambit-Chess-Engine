#ifndef GAME_HISTORY_HPP
#define GAME_HISTORY_HPP

#include "Types.hpp"

class Game_History {
    public:
        Game_History();
        void add(u64 key);
        void pop();
        void clear();
        u64 get_element(int i);

        int get_index();
        
    private:
        u64 stack[100] = {}; // initialise stack to hold all 0s
        int index;
};

#endif // #ifndef GAME_HISTORY_HPP