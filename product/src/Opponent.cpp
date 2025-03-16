#include "Opponent.hpp"

Opponent::Opponent(){}

void Opponent::found_best_move() { skill += 100; }

void Opponent::not_found_best_move() { skill -= 100; }

int Opponent::calculate_risk_to_reward(int eval)
{ // probably dont want to deal with floats so will force it into an int
    int risk = calculate_risk();
    return risk/eval;
}

int Opponent::calculate_risk() { return 0; }

bool Opponent::should_play(int risk, int reward)
{ // if success is 3 times better than the guranteed return of AB, this move is promising.
    if(risk > reward) {
    //if(risk > reward * 3) {
        return true;
    }
    return false;
}

int Opponent::calculate_likelihood(Position& pos, Move& move, int score) {
    return 1;
    // if(move.get_flag() == Move_Flag::KNIGHT_FLAG)
    // { // minus reinforces these moves for some reason
    //     return -10000;
    // }
    // else { return 1; }
}

Tactic Opponent::get_tactic(Move& move)
{
    // For each tactic, find if it exists?

    // Moves already have various move types like promotions, castling etc
    // Detecting noisy moves would be good for Q search
}

int Opponent::approach_1(int ABE, int FLE, int SLE)
{
    return ((ABE - FLE) / (SLE - ABE));
}
