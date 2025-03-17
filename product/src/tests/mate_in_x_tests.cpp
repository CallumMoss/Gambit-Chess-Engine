#include "../Types.hpp"
#include "../Position.hpp"
#include "../utils.hpp"
#include "../Magics.hpp"
#include "../Search.hpp"
#include "../Opponent.hpp"

void test_iterative_deepening_opponent_mate_in_1() {
    // Positions from: https://lichess.org/study/IPtfJlNl
    Magics::init();
    Position pos = Position("4r2k/1p3rbp/2p1N1p1/p3n3/P2NB1nq/1P6/4R1P1/B1Q2RK1 b - - 4 32");
    Opponent opp;
    Search search = Search(false, opp);
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
    Move expected_move = Move(31, 15, Move_Flag::QUEEN_FLAG);
    std::cout << Utils::move_to_board_notation(search.get_root_best_move()) << std::endl;
    assert(search.get_root_best_move().equals(expected_move));
    std::cout << "Test suite: test_negamax_iterative_deepening_opponent_mate_in_1: Test 1 passed\n";
  
    pos = Position("4rb2/3qrk2/1p1p1n2/7p/P2P4/4R2P/1BQN1P2/1K4R1 w - - 3 39");
    Search search2 = Search(false, opp);
    timer = Timer();
    ps = PositionStack();
    tt = Transposition_Table();
    tt.resize(16);
    wtime = 60'000; // white has x msec left on the clock
    btime = 60'000; // black has x msec left on the clock
    winc = 0; // white increment per move in mseconds if x > 0
    binc = 0; // black increment per move in mseconds if x > 0
    if(pos.get_turn() == Turn::WHITE) {
        timer.set_fields(wtime, winc);
    }
    else {
        timer.set_fields(btime, binc);
    }
    timer.start_timer();
    search2.iterative_deepening(pos, timer, tt, ps);
    expected_move = Move(10, 46, Move_Flag::QUEEN_FLAG);
    std::cout << Utils::move_to_board_notation(search2.get_root_best_move()) << std::endl;
    assert(search2.get_root_best_move().equals(expected_move));
    std::cout << "Test suite: test_negamax_iterative_deepening_opponent_mate_in_1: Test 2 passed\n";
  
    pos = Position("r1bqkbnr/pp2pp1p/3p2p1/2p5/3nP3/2N3PP/PPP1NP2/R1BQKB1R b KQkq - 1 6");
    Search search3 = Search(false, opp);
    timer = Timer();
    ps = PositionStack();
    tt = Transposition_Table();
    tt.resize(16);
    wtime = 60'000; // white has x msec left on the clock
    btime = 60'000; // black has x msec left on the clock
    winc = 0; // white increment per move in mseconds if x > 0
    binc = 0; // black increment per move in mseconds if x > 0
    if(pos.get_turn() == Turn::WHITE) {
        timer.set_fields(wtime, winc);
    }
    else {
        timer.set_fields(btime, binc);
    }
    timer.start_timer();
    search3.iterative_deepening(pos, timer, tt, ps);
    ps = PositionStack();
    tt = Transposition_Table();
    tt.resize(16);
    expected_move = Move(27, 21, Move_Flag::KNIGHT_FLAG);
    std::cout << Utils::move_to_board_notation(search3.get_root_best_move()) << std::endl;
    assert(search3.get_root_best_move().equals(expected_move));
    std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_1: Test 3 passed\n";
  
    pos = Position("r2q1rk1/1bppb1pp/n3p3/5P2/2BP1P2/4P3/1PPnQK1P/R1B3NR b - - 1 12");
    Search search4 = Search(false, opp);
    timer = Timer();
    ps = PositionStack();
    tt = Transposition_Table();
    tt.resize(16);
    wtime = 60'000; // white has x msec left on the clock
    btime = 60'000; // black has x msec left on the clock
    winc = 0; // white increment per move in mseconds if x > 0
    binc = 0; // black increment per move in mseconds if x > 0
    if(pos.get_turn() == Turn::WHITE) {
        timer.set_fields(wtime, winc);
    }
    else {
        timer.set_fields(btime, binc);
    }
    timer.start_timer();
    search4.iterative_deepening(pos, timer, tt, ps);
    expected_move = Move(52, 31, Move_Flag::BISHOP_FLAG);
    std::cout << Utils::move_to_board_notation(search4.get_root_best_move()) << std::endl;
    assert(search4.get_root_best_move().equals(expected_move));
    std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_1: Test 4 passed\n";
  
    pos = Position("3q1rk1/1bp3pp/r3P3/3p4/3PnP1b/1P2P3/2PNQ2P/R1BK2NR b - - 0 15");
    Search search5 = Search(false, opp);
    timer = Timer();
    ps = PositionStack();
    tt = Transposition_Table();
    tt.resize(16);
    wtime = 60'000; // white has x msec left on the clock
    btime = 60'000; // black has x msec left on the clock
    winc = 0; // white increment per move in mseconds if x > 0
    binc = 0; // black increment per move in mseconds if x > 0
    if(pos.get_turn() == Turn::WHITE) {
        timer.set_fields(wtime, winc);
    }
    else {
        timer.set_fields(btime, binc);
    }
    timer.start_timer();
    search5.iterative_deepening(pos, timer, tt, ps);
    expected_move = Move(28, 18, Move_Flag::KNIGHT_FLAG);
    std::cout << Utils::move_to_board_notation(search5.get_root_best_move()) << std::endl;
    assert(search5.get_root_best_move().equals(expected_move));
    std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_1: Test 5 passed\n";
  }
  
  void test_iterative_deepening_opponent_mate_in_2() {
    Magics::init();
    Position pos = Position("4r3/1pp2rbk/6pn/4n3/P3BN1q/1PB2bPP/8/2Q1RRK1 b - - 0 31");
    Opponent opp;
    Search search = Search(false, opp);
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
    Move expected_move = Move(31, 22, Move_Flag::QUEEN_FLAG);
    assert(search.get_root_best_move().equals(expected_move));
    std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_2: Test 1 passed\n";
  
    pos = Position("r2k1b1r/p1ppq2p/np3np1/5p2/1PPP4/P3PQ2/3N1PPP/R1B1K2R w KQ - 1 13");
    Search search2 = Search(false, opp);
    timer = Timer();
    ps = PositionStack();
    tt = Transposition_Table();
  tt.resize(16);
    wtime = 60'000; // white has x msec left on the clock
    btime = 60'000; // black has x msec left on the clock
    winc = 0; // white increment per move in mseconds if x > 0
    binc = 0; // black increment per move in mseconds if x > 0
    if(pos.get_turn() == Turn::WHITE) {
        timer.set_fields(wtime, winc);
    }
    else {
        timer.set_fields(btime, binc);
    }
    timer.start_timer();
    search2.iterative_deepening(pos, timer, tt, ps);
    expected_move = Move(21, 56, Move_Flag::QUEEN_FLAG);
    assert(search2.get_root_best_move().equals(expected_move));
    std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_2: Test 2 passed\n";
  
    pos = Position("rn3r2/p2q1pBk/1p2p3/3pP3/P1bNnQ2/5NP1/1P3PBP/R3b1K1 w - - 1 19");
    Search search3 = Search(false, opp);
    timer = Timer();
    ps = PositionStack();
    tt = Transposition_Table();
    tt.resize(16);
    wtime = 60'000; // white has x msec left on the clock
    btime = 60'000; // black has x msec left on the clock
    winc = 0; // white increment per move in mseconds if x > 0
    binc = 0; // black increment per move in mseconds if x > 0
    if(pos.get_turn() == Turn::WHITE) {
        timer.set_fields(wtime, winc);
    }
    else {
        timer.set_fields(btime, binc);
    }
    timer.start_timer();
    search3.iterative_deepening(pos, timer, tt, ps);
    expected_move = Move(29, 47, Move_Flag::QUEEN_FLAG);
    assert(search3.get_root_best_move().equals(expected_move));
    std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_2: Test 3 passed\n";
}