#include "Game_History.hpp"

Game_History::Game_History(){ stack.reserve(255); }

void Game_History::add(u64 key) {
    stack.push_back(key);
}

void Game_History::pop() {
    stack.pop_back();
}

u64 Game_History::get_element(int i) {
    return stack[i];
}

void Game_History::clear() {
    for(int i = 0; i < stack.size(); i++) {
        stack[i] = 0;
    }
}