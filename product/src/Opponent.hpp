#ifndef OPPONENT_HPP
#define OPPONENT_HPP

#include "Types.hpp"
#include "utils.hpp"
#include "Position.hpp"

// Could be represented in a bitstring, but too complex to detect many tactics at once
enum Tactic {
    fork, // most common
    pin,
    battery,
    check,
    sacrifice,
    en_passant,
    discovered_attack,
    zugzwang // if all moves.eval makes eval worse. Unlikely to be known to most players though.
};

/**
 * @brief Models an opponent and stores key game stats
 * 
 */
class Opponent
{
    public:
        Opponent();
        Opponent(Turn colour);
        Opponent(Turn colour, int skill);

        void update_skill(int move_ranking, int move_list_size);

        int calculate_risk_to_reward(int eval);

        int calculate_risk();

        bool should_play(int risk, int reward);

        /**
         * @brief Calculautes the likelihood of a move being played
         * 
         * @return int between 0 and 100 using a normalizer function
         */
        int calculate_likelihood(Position& pos, Move& move, int score);

        Tactic get_tactic(Move& move);

        /**
         * @brief If a line likelihood is above NSL, store in a list and pick the highest SLE:Sucess
         * 
         * @param ABE alpha beta evaluation
         * @param FLE failed line evaluation
         * @param SLE successful line evaluation
         * @return int Necessary line success likelihood
         */
        int approach_1(int ABE, int FLE, int SLE);

        Turn get_colour() { return colour; }
        void set_colour(Turn colour) { this->colour = colour; }

        int get_skill() { return skill; }
        int get_max_skill() { return max_skill; }
        float get_probability_of_optimal()
        { // Could get elo from Lichess, and use our elo as a baseline for 100%.
            // Ensure skill used in computation is within ranges for calculation
            int temp_skill = std::min(skill, max_skill);
            temp_skill = std::max(temp_skill, 0);
            return (temp_skill / max_skill); // max probability of 1
        }

    private:
        int max_skill = 10000;
        int skill = max_skill; // assume optimal play from the start
        Turn colour; // colour of the opponent
};

#endif // OPPONENT_HPP