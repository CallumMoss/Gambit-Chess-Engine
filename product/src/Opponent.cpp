#include "Opponent.hpp"

Opponent::Opponent(){} // default constructor

Opponent::Opponent(Turn colour)
{
    this->colour = colour;
}

Opponent::Opponent(Turn colour, int skill)
{
    this->colour = colour;
    this->skill = skill;
}

void Opponent::update_skill(int move_ranking, int move_list_size)
{
    assert(move_ranking < move_list_size);
    assert(move_ranking >= 0);
    int skill_adjustment_weight = 100;
    // index at move_list_size / 2 is 0. Then adjust accordingly.
    int middle = move_list_size / 2;
    
    // Example: move_list_size = 40
    // if best move, where index is 0, then skill += skill_adjustment_weight * (20 - 0), which if 100 then increased by 2000.
    // if worst move, index is 39, then skill += skill_adjustment_weight * (20 - 39), which if 100 then decreased by 1900.
    // if somewhere near the middle, then minor increase or decrease to score.
    skill += skill_adjustment_weight * (middle - move_ranking);
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
