#include <gtest/gtest.h>
#include <iostream>

#include "../Types.hpp"
#include "../Position.hpp"
#include "../utils.hpp"
#include "../Magics.hpp"
#include "../Search.hpp"
#include "../Opponent.hpp"
#include "Perft.hpp"

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

#include "../RunGambit.hpp"

// TEST(Opponent_Modelling, Expectiminimax)
// {  
//   // std::vector<std::string> commands =
//   // {
//   //   "go", "position startpos moves b1c3"
//   // };
//   // run_gambit(commands);
//   Opponent opp;
//   Position pos;
//   Move move_played = Move(1, 18, Move_Flag::KNIGHT_FLAG);

//   Position new_pos = pos;
//   new_pos.make_move(move_played);
//   std::vector<Move> responses = new_pos.generate_all_moves(false);
//   std::vector<EvaluatedMove> evaluated_responses;
//   for(Move move : responses)
//   {
//       Position temp_pos = new_pos;
//       temp_pos.make_move(move);
//       if(temp_pos.is_legal(move))
//       {
//           EvaluatedMove eval_move = EvaluatedMove(move, Evaluation::evaluate(temp_pos));
//           evaluated_responses.push_back(eval_move);
//       }
//   }
 
//   // now sort by eval
//   std::sort(evaluated_responses.begin(), evaluated_responses.end(), [](const EvaluatedMove& a, const EvaluatedMove& b) {
//       return a.eval > b.eval; // Higher scores come first
//   });
  
//   for(EvaluatedMove em : evaluated_responses)
//   {
//     std::cout << Utils::move_to_board_notation(em.move) << std::endl;
//     std::cout << em.eval << std::endl;
//   }
//   //Move response =  // best response
// doesnt work because doesnt look into enough depth to get something reasonable so should run main
// }

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  Zobrist z;
  Magics::init();
  return RUN_ALL_TESTS();
}