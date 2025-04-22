#include <gtest/gtest.h>
#include <iostream>

#include "../Types.hpp"
#include "../Position.hpp"
#include "../utils.hpp"
#include "../Magics.hpp"
#include "../Search.hpp"
#include "../Opponent.hpp"
#include "Perft.hpp"
#include "../RunGambit.hpp"

// Test files:
#include "fen_parsing_tests.hpp"
#include "mate_in_x_tests.hpp"
#include "zobrist_tests.hpp"
//

/*
TEST(Fen_Parsing, testing_fen)
{
    parse_initial_pos();
    parse_mid_game();
    parse_castling();
    parse_complex_pos();
}

TEST(PERFT, perft_suite_normal)
{
  Perft::run_perft_suite(false, false);
}

TEST(PERFT, perft_suite_qsearch)
{
  // Gotten from https://www.chessprogramming.org/Perft_Results
  // Where expected noisy size is number of captures minus number of en passant moves
  Perft::run_perft_suite(false, true);
}

TEST(Mate_In_X, testing_mates)
{
    test_iterative_deepening_opponent_mate_in_1();
    test_iterative_deepening_opponent_mate_in_2();
}

TEST(Zobrist_Hashing, testing_zobrist)
{
    test_zobrist_1();
    test_zobrist_2();
    test_zobrist_3();
    test_zobrist_4();

    test_zobrist_recomputation_1();
    test_zobrist_recomputation_2();
    test_zobrist_recomputation_3();
    test_zobrist_recomputation_4();
    test_zobrist_recomputation_5();
    test_zobrist_recomputation_6();
    test_zobrist_recomputation_7();
    test_zobrist_recomputation_8();
    test_zobrist_recomputation_9();
}
*/

TEST(Opponent_Modelling, Skill_Updating)
{
  // Increase
  Opponent opp;
  int move_ranking = 0;
  int move_list_size = 20;
  opp.update_skill(move_ranking, move_list_size);
  std::cout << opp.get_skill() << std::endl;

  // Decrease
  opp = Opponent();
  move_ranking = 19;
  move_list_size = 20;
  opp.update_skill(move_ranking, move_list_size);
  std::cout << opp.get_skill() << std::endl;
  //assert(opp.get_skill() == (opp.get_max_skill()))
}

// TEST(Opponent_Modelling, Move_Evaluation)
// { // Testing in a mate in one pos

//   // Move that invoked this position that we played
//   Move us_move = Move(9, 0, Move_Flag::BISHOP_FLAG);

//   Position pos = Position("4r2k/1p3rbp/2p1N1p1/p3n3/P2NB1nq/1P6/1B2R1P1/2Q2RK1 w - - 4 32");
//   // when us move is applied, we reach the mate in 1 position to evaluate

//   Opponent opp;
//   Search s = Search(true, opp);
//   s.set_root_best_move(us_move);

//   std::vector<EvaluatedMove> responses = s.get_evaluated_responses(pos);
//   for(EvaluatedMove em : responses)
//   {
//     std::cout << Utils::move_to_board_notation(em.move) << " | " << em.eval << std::endl;
//   }
  
//   // // Increase
//   // int move_ranking = 0;
//   // int move_list_size = 20;
//   // opp.update_skill(move_ranking, move_list_size);
//   // std::cout << opp.get_skill() << std::endl;

//   // // Decrease
//   // opp = Opponent();
//   // move_ranking = 19;
//   // move_list_size = 20;
//   // opp.update_skill(move_ranking, move_list_size);
//   // std::cout << opp.get_skill() << std::endl;
//   // //assert(opp.get_skill() == (opp.get_max_skill()))
// }

/*
  instead of finding the opps ranked moves after search which would result in O(2n), where n is the complexity of search
  we can do it in 0(kn), where k is a constant time operation to perform this extra step.
  We rank the opponents best moves throughout the search process
*/

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  Zobrist z;
  Magics::init();
  return RUN_ALL_TESTS();
}