#include "Game_History.hpp"

Game_History::Game_History(){}

void Game_History::add(u64 key) {
    stack[index] = key;
    index++;
}

void Game_History::pop() {
    stack[index] = 0;
    index--;
}

u64 Game_History::get_element(int i) {
    return stack[i];
}

void Game_History::clear() {
    for(int i = 0; i < 100; i++) {
        stack[i] = 0;
    }
}

int Game_History::get_index() {
    return index;
}