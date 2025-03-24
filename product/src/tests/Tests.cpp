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

// 3 Steps:
// 1. Adjusting opponent skill
// 2. Playing moves based off skill
// 3. Fine tune skill updates

TEST(Opponent_Modelling, Evaluating_Strength)
{
//only happens once per opponent move, so can be computationally expensive
  //   Move gambit_best_move = Move(12, 28, Move_Flag::PAWN_TWO_FORWARD_FLAG); //e2e4
  //   Move opponents_move = Move(11, 27, Move_Flag::PAWN_TWO_FORWARD_FLAG); //d2d4

  // very unlikely to find best move

  // made  move that made their position worse. Doesnt mean that theyre a bad player.
  // unless zugzwang

  //   Opponent opp;
  //   opp.update_skill(gambit_best_move.equals(opponents_move));

  //     // - some amount of eval during search based on opponent skill
}

*/

TEST(Opponent_Modelling, Expectimax)
{
  // Now we can adjust opponent score, lets pick a move based on said score
    Position pos = Position();
    Opponent opp = Opponent(Turn::BLACK);
    Search search = Search(true, opp);
    Timer timer;
    PositionStack ps;
    Transposition_Table tt;
    tt.resize(16);
    u64 wtime = 60'000; // white has x msec left on the clock
    u64 btime = 60'000; // black has x msec left on the clock
    u64 winc = 0; // white increment per move in mseconds if x > 0
    u64 binc = 0; // black increment per move in mseconds if x > 0
    if(pos.get_turn() == Turn::WHITE) {
        timer.set_fields(wtime, winc);
    }
    else {
        timer.set_fields(btime, binc);
    }
    timer.start_timer();
    search.iterative_deepening(pos, timer, tt, ps);
    //search.expectiminimax(2, 0, pos, timer);
    // Move expected_move = Move(31, 15, Move_Flag::QUEEN_FLAG);
    std::cout << Utils::move_to_board_notation(search.get_root_best_move()) << std::endl;
    // assert(search.get_root_best_move().equals(expected_move));
    // std::cout << "Test suite: test_negamax_iterative_deepening_opponent_mate_in_1: Test 1 passed\n";
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  Zobrist z;
  Magics::init();
  return RUN_ALL_TESTS();
}