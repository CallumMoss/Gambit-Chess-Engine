#ifndef OPPONENT_HPP
#define OPPONENT_HPP

// Could be represented in a bitstring, but too complex to detect many tactics at once
// enum Tactic {
//     how do engines percieve tactics? once this is the case,
//     I can get more complex stuff going.
//     Perhaps neural net is the only way to go though
//     Maybe check if patricia for example was trained to go for short games
//     fork,
//     pin,
//     battery,
//     check,
//     sacrafice,
//     en_passant,
//     discovered_attack
// };

// Used to form an opponent model
// This is used to determine whether a move is likely to be found
// This can be used to determine the risk to reward payoff.

/**
 * @brief Models an opponent and stores key game stats
 * 
 */
class Opponent {
    // Temporary rudimentary approach
    public:
        void found_best_move() { skill += 100; }
        void not_found_best_move() { skill -= 100; }
        int calculate_risk_to_reward(int eval) { // probably dont want to deal with floats so will force it into an int
            int risk = calculate_risk();
            return risk/eval;
        }
        int calculate_risk() { return 0; }

        bool should_play0(int risk, int reward) { // if success is 3 times better than the guranteed return of AB, this move is promising.
            if(risk > reward) {
            //if(risk > reward * 3) {
                return true;
            }
            return false;
        }

        /**
         * @brief Calculautes the likelihood of a move being played
         * 
         * @return int between 0 and 100 using a normalizer function
         */
        int calculate_likelihood(Position& pos, Move& move) {
            return 50;
        }

        // Tactic get_tactic(Move& move) {
        //     // Moves already have various move types like promotions, castling etc
        //     // Detecting noisy moves would be good for Q search
        // }

        /**
         * @brief If a line likelihood is above NSL, store in a list and pick the highest SLE:Sucess
         * 
         * @param ABE alpha beta evaluation
         * @param FLE failed line evaluation
         * @param SLE successful line evaluation
         * @return int Necessary line success likelihood
         */
        int approach_1(int ABE, int FLE, int SLE) {
            return ((ABE - FLE) / (SLE - ABE));
        }

    private:
        int skill = 1000; // defaults to 1000 and is updated throughout the game'
        Turn colour; // colour of the opponent
};

/*
    To Do:
    Implement a new search algorithm which does not skip "bad" moves.
    It should store a dictionary of moves with their corresponding evaluation
    Then sort in a priority queue, where likelihood is at the top?

    Many approaches:
    1. For each move, determine the likelihood the opponent is to play a move (when searching for opponents turn in search tree)
    Determine also the eval for each opponent move, then instead of returning eval, we return eval * likelihood or something.

    2. Find best case and worst case (from engine POVs), then compare the likelihood.
    
    What would I like to achieve?
    I want to determine which move to play.
    This move should have some kind of risk to reward payoff, 
    such that we favour high rewards for low risk.
    The base comparison for a move should be the best move
    This will have a risk of 0 and reward of 0 (or of its eval?)
    Make an equation that compares risk to reward.
    Generally we want a risk to reward for 1/3 (as proposed with stocks)
    However this can be fine tuned through many games and checking short game stats.

    I will prototype and test vs myself (choosing to sometimes make mistakes)
    To test which approach is best


    risk to reward can be seen as worst outcome : best outcome

    There will be n moves at any position, so we store in a list moves
    we will build a custom data type to store the eval

    Implement approach 1, then 2
*/
#endif // OPPONENT_HPP