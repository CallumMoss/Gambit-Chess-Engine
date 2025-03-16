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

        void found_best_move();

        void not_found_best_move();

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

    private:
        int skill = 1000; // defaults to 1000 and is updated throughout the game
        Turn colour = Turn::WHITE; // colour of the opponent
};

#endif // OPPONENT_HPP