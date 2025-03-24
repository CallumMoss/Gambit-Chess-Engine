#include "Opponent.hpp"

Opponent::Opponent(Turn colour)
{
    this->colour = colour;
}

Opponent::Opponent(Turn colour, int skill)
{
    this->colour = colour;
    this->skill = skill;
}

void Opponent::update_skill(bool found_best_move)
{
    // Average chess game is 40 moves.
    // We do not want to assume a below 50% chance of finding the best move
    // Therefore, (100 - 50) / 40 = 1.25. Dealing with floats is not ideal 
    // So must increase magnitude by 100.
     skill += found_best_move ? 125 : -125; // times by some weight based on difficulty of finding move
}

int Opponent::calculate_risk_to_reward(int eval)
{ // probably dont want to deal with floats so will force it into an int
    int risk = calculate_risk();
    return risk/eval;
}

int Opponent::calculate_risk() { return 0; }

bool Opponent::should_play(int risk, int reward)
{ // if success is 3 times better than the guranteed return of AB, this move is promising.
    return risk > reward * 3;
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
