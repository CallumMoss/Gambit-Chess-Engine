#include <gtest/gtest.h>
#include <iostream>

#include "../Types.hpp"
#include "../Position.hpp"
#include "../utils.hpp"
#include "../Magics.hpp"
#include "../Search.hpp"
#include "../Opponent.hpp"
#include "Perft.hpp"

// // // Testing getters and FEN parser.
// // TEST(FEN_And_Pos_Rep, initial_position) {
// //   Position pos = Position();
// //   EXPECT_TRUE(pos.get_wscr()) << "Expected white to have short castling rights, but it did not.";
// //   EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
// //   EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
// //   EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

// //   EXPECT_EQ(pos.get_pawns(), 0xff00000000ff00ULL) << "Pawns are not in the expected position.";
// // 	EXPECT_EQ(pos.get_knights(), 0x4200000000000042ULL) << "Knights are not in the expected position.";
// // 	EXPECT_EQ(pos.get_bishops(), 0x2400000000000024ULL) << "Bishops are not in the expected position.";
// // 	EXPECT_EQ(pos.get_rooks(), 0x8100000000000081ULL) << "Rooks are not in the expected position.";
// // 	EXPECT_EQ(pos.get_queens(), 0x800000000000008ULL) << "Queens are not in the expected position.";
// // 	EXPECT_EQ(pos.get_kings(), 0x1000000000000010ULL) << "Kings are not in the expected position.";

// //   EXPECT_EQ(pos.get_white_pawns(), 0xff00ULL) << "The white pawns are not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_pawns(), 0xff000000000000ULL) << "The black pawns are not in the expected position.";
// //   EXPECT_EQ(pos.get_white_knights(), 0x42ULL) << "The white knights are not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_knights(), 0x4200000000000000ULL) << "The black knights are not in the expected position.";
// //   EXPECT_EQ(pos.get_white_bishops(), 0x24ULL) << "The white bishops are not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_bishops(), 0x2400000000000000ULL) << "The black bishops are not in the expected position.";
// //   EXPECT_EQ(pos.get_white_rooks(), 0x81ULL) << "The white rooks are not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_rooks(), 0x8100000000000000ULL) << "The black rooks are not in the expected position.";
// //   EXPECT_EQ(pos.get_white_queen(), 0x8ULL) << "The white queen is not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_queen(), 0x800000000000000ULL) << "The black queen is not in the expected position.";
// //   EXPECT_EQ(pos.get_white_king(), 0x10ULL) << "The white king is not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_king(), 0x1000000000000000ULL) << "The black king is not in the expected position.";

// //   EXPECT_EQ(pos.get_white_pieces(), 0xffffULL) << "The white pieces are not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_pieces(), 0xffff000000000000ULL) << "The black pieces are not in the expected position.";
// //   EXPECT_EQ(pos.get_board(), 0xffff00000000ffffULL) << "The pieces are not in the expected position.";

// // 	EXPECT_EQ(pos.get_turn(), Turn::WHITE) << "Expected it to be white's turn, but it is not.";
// // 	EXPECT_EQ(pos.get_en_passant_target(), Utils::NULL_EN_PASSANT) << "Expected no en passant posibility, but there is.";
// // 	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
// // 	EXPECT_EQ(pos.get_full_move_counter(), 1) << "Expected the full move clock to be 1, but it is not.";
// // }

// // // We now know that getters work for getting specific coloured pieces. So only need to test pieces and colours indiviually.
// // TEST(FEN_And_Pos_Rep, typical_middle_game) {
// //   Position pos = Position("r3k2r/2pb1ppp/2pp1q2/p7/1nP1B3/1P2P3/P2N1PPP/R2QK2R w KQkq - 0 14");
// //   EXPECT_TRUE(pos.get_wscr()) << "Expected white to have short castling rights, but it did not.";
// //   EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
// //   EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
// //   EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

// //   EXPECT_EQ(pos.get_pawns(), 0xe40c010412e100ULL) << "Pawns are not in the expected position.";
// // 	EXPECT_EQ(pos.get_knights(), 0x2000800ULL) << "Knights are not in the expected position.";
// // 	EXPECT_EQ(pos.get_bishops(), 0x8000010000000ULL) << "Bishops are not in the expected position.";
// // 	EXPECT_EQ(pos.get_rooks(), 0x8100000000000081ULL) << "Rooks are not in the expected position.";
// // 	EXPECT_EQ(pos.get_queens(), 0x200000000008ULL) << "Queens are not in the expected position.";
// // 	EXPECT_EQ(pos.get_kings(), 0x1000000000000010ULL) << "Kings are not in the expected position.";

// //   EXPECT_EQ(pos.get_white_pieces(), 0x1412e999ULL) << "The white pieces are not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_pieces(), 0x91ec2c0102000000ULL) << "The black pieces are not in the expected position.";

// // 	EXPECT_EQ(pos.get_turn(), Turn::WHITE) << "Expected it to be white's turn, but it is not.";
// // 	EXPECT_EQ(pos.get_en_passant_target(), Utils::NULL_EN_PASSANT) << "Expected no en passant posibility, but there is.";
// // 	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
// // 	EXPECT_EQ(pos.get_full_move_counter(), 14) << "Expected the full move clock to be 14, but it is not.";
// // }

// // // Testing slightly middle game with a half move, blacks turn and white having castled.
// // TEST(FEN_And_Pos_Rep, middle_game_white_castled) {
// //   Position pos = Position("r1bqk2r/ppp2ppp/2n1pn2/3p4/3P4/2NBPN2/PPPB1PPP/R2Q1RK1 b kq - 1 7");
// //   EXPECT_FALSE(pos.get_wscr()) << "Expected white to not have short castling rights, but it did.";
// //   EXPECT_FALSE(pos.get_wlcr()) << "Expected white to not have long castling rights, but it did.";
// //   EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
// //   EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

// //   EXPECT_EQ(pos.get_pawns(), 0xe710080810e700ULL) << "Pawns are not in the expected position.";
// // 	EXPECT_EQ(pos.get_knights(), 0x240000240000ULL) << "Knights are not in the expected position.";
// // 	EXPECT_EQ(pos.get_bishops(), 0x400000000080800ULL) << "Bishops are not in the expected position.";
// // 	EXPECT_EQ(pos.get_rooks(), 0x8100000000000021ULL) << "Rooks are not in the expected position.";
// // 	EXPECT_EQ(pos.get_queens(), 0x800000000000008ULL) << "Queens are not in the expected position.";
// // 	EXPECT_EQ(pos.get_kings(), 0x1000000000000040ULL) << "Kings are not in the expected position.";

// //   EXPECT_EQ(pos.get_white_pieces(), 0x83cef69ULL) << "The white pieces are not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_pieces(), 0x9de7340800000000ULL) << "The black pieces are not in the expected position.";

// // 	EXPECT_EQ(pos.get_turn(), Turn::BLACK) << "Expected it to be white's turn, but it is not.";
// // 	EXPECT_EQ(pos.get_en_passant_target(), Utils::NULL_EN_PASSANT) << "Expected no en passant posibility, but there is.";
// // 	EXPECT_EQ(pos.get_half_move_clock(), 1) << "Expected the half move clock to be 1, but it is not.";
// // 	EXPECT_EQ(pos.get_full_move_counter(), 7) << "Expected the full move clock to be 7, but it is not.";
// // }

// // // Tests en passant, blacks turn and white castling after moving rook
// // TEST(FEN_And_Pos_Rep, complex_position) {
// //   Position pos = Position("2kr3r/pb1pppbp/1pn5/q1p1n3/2BP2pP/PPN1BN2/2PQ1PP1/R3KR2 b Q h3 0 14");
// //   EXPECT_FALSE(pos.get_wscr()) << "Expected white to not have short castling rights, but it did.";
// //   EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
// //   EXPECT_FALSE(pos.get_bscr()) << "Expected black to have short castling rights, but it did.";
// //   EXPECT_FALSE(pos.get_blcr()) << "Expected black to have long castling rights, but it did.";

// //   EXPECT_EQ(pos.get_pawns(), 0xb90204c8036400ULL) << "Pawns are not in the expected position.";
// // 	EXPECT_EQ(pos.get_knights(), 0x41000240000ULL) << "Knights are not in the expected position.";
// // 	EXPECT_EQ(pos.get_bishops(), 0x42000004100000ULL) << "Bishops are not in the expected position.";
// // 	EXPECT_EQ(pos.get_rooks(), 0x8800000000000021ULL) << "Rooks are not in the expected position.";
// // 	EXPECT_EQ(pos.get_queens(), 0x100000800ULL) << "Queens are not in the expected position.";
// // 	EXPECT_EQ(pos.get_kings(), 0x400000000000010ULL) << "Kings are not in the expected position.";

// //   EXPECT_EQ(pos.get_white_pieces(), 0x8c376c31ULL) << "The white pieces are not in the expected position.";
// // 	EXPECT_EQ(pos.get_black_pieces(), 0x8cfb061540000000ULL) << "The black pieces are not in the expected position.";

// // 	EXPECT_EQ(pos.get_turn(), Turn::BLACK) << "Expected it to be white's turn, but it is not.";
// // 	EXPECT_EQ(pos.get_en_passant_target(), 23) << "Expected en passant on h3 but there is not";
// // 	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
// // 	EXPECT_EQ(pos.get_full_move_counter(), 14) << "Expected the full move clock to be 14, but it is not.";
// // }

// // // ***** Partial Perft: ***** //

// // // To debug using perft and web perft:
// // // Find up to a depth where you are accurate
// // // then for the first failed depth, compare the difference of your results by using if statement for the move and printing moves
// // // Go down the line of moves with incorrect nodes till you get to the original moves that were wrong at depth 1
// // // Solve the problem
// // TEST(PERFT, pos1) { // starting position
// // 	Position pos = Position();
// // 	int depth = 6; // where depth is measured in half moves / plys
// // 	std::cout << "Position 1: (Depth " << depth << ")" << std::endl;
// //   u64 total_nodes = Perft::split_perft(depth, depth, true, Utils::NULL_MOVE, pos);
// //   std::cout << total_nodes << std::endl;
// //   assert(total_nodes == 119060324);
// // }

// // TEST(PERFT, pos2) { // conventionally named kiwipete
// // 	Position pos = Position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
// // 	int depth = 5; // where depth is measured in half moves / plys
// //   u64 total_nodes = Perft::split_perft(depth, depth, true, Utils::NULL_MOVE, pos);
// //   std::cout << total_nodes << std::endl;
// //   assert(total_nodes == 193690690);
// // }

// // TEST(PERFT, pos3) {
// // 	Position pos = Position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
// // 	int depth = 7; // where depth is measured in half moves / plys
// //   u64 total_nodes = Perft::split_perft(depth, depth, true, Utils::NULL_MOVE, pos);
// //   std::cout << total_nodes << std::endl;
// //   assert(total_nodes == 178633661);
// // }

// // TEST(PERFT, pos4) {
// // 	Position pos = Position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
// // 	int depth = 5; // where depth is measured in half moves / plys
// //   u64 total_nodes = Perft::split_perft(depth, depth, true, Utils::NULL_MOVE, pos);
// //   std::cout << total_nodes << std::endl;
// //   assert(total_nodes == 15833292);
// // }

// // TEST(PERFT, pos5) {
// // 	Position pos = Position("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
// // 	int depth = 5; // where depth is measured in half moves / plys
// //   u64 total_nodes = Perft::split_perft(depth, depth, true, Utils::NULL_MOVE, pos);
// //   std::cout << total_nodes << std::endl;
// //   assert(total_nodes == 89941194);
// // }

// // TEST(PERFT, pos6) {
// // 	Position pos = Position("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
// // 	int depth = 5; // where depth is measured in half moves / plys
// //   u64 total_nodes = Perft::split_perft(depth, depth, true, Utils::NULL_MOVE, pos);
// //   std::cout << total_nodes << std::endl;
// //   assert(total_nodes == 164075551);
// // }

// // *****                ***** //

// TEST(SEARCH, mvv_lva) {
//   Piece victim = Piece::PAWN;
//   Piece attacker = Piece::QUEEN;
//   Opponent opp;
//   Search s = Search(false, opp);
// 	std::cout << s.find_mvv_lva(victim, attacker) << std::endl; // 45
// }

// TEST(SEARCH, mvv_lva2) {
//   Piece victim = Piece::QUEEN;
//   Piece attacker = Piece::PAWN;
//   Opponent opp;
//   Search s = Search(false, opp);
// 	std::cout << s.find_mvv_lva(victim, attacker) << std::endl; // 9
// }

// TEST(SEARCH, mvv_lva3) {
//   Piece victim = Piece::KING;
//   Piece attacker = Piece::PAWN;
//   Opponent opp;
//   Search s = Search(false, opp);
//   	ASSERT_EXIT(
//     { std::cerr << s.find_mvv_lva(victim, attacker) << std::endl; },
//     ::testing::ExitedWithCode(-1),                                        
//     "Unexpected piece type of victim: 5"                   
//   );
// }

// TEST(SEARCH, mvv_lva4) { // lowest
//   Piece victim = Piece::QUEEN;
//   Piece attacker = Piece::KING;
//   Opponent opp;
//   Search s = Search(false, opp);
// 	std::cout << s.find_mvv_lva(victim, attacker) << std::endl; // 4
// }

// TEST(SEARCH, mvv_lva5) { // highest
//   Piece victim = Piece::PAWN;
//   Piece attacker = Piece::PAWN;
//   Opponent opp;
//   Search s = Search(false, opp);
// 	std::cout << s.find_mvv_lva(victim, attacker) << std::endl; // 49
// }

// TEST(SEARCH, mvv_lva_sorting) { // testing whether sorted returns the correct number of moves
//   Position pos = Position("5k2/7p/6p1/8/8/8/8/3K3R w - - 0 1");
//   Magics::init();
//   std::vector<Move> moves = pos.generate_all_moves(false);
//   Opponent opp;
//   Search s = Search(false, opp);
//   std::vector<Move> sorted_moves = s.sort_by_mvv_lva(moves, pos);
//   Piece victim = Piece::PAWN;
//   Piece attacker = Piece::ROOK;
//   assert(s.find_mvv_lva(victim, attacker) == 46);
//   assert(moves.size() == sorted_moves.size());
//   //std::cout << Utils::move_to_board_notation(sorted_moves[static_cast<int>(sorted_moves.size()) - 1]) << std::endl;
//   assert(static_cast<int>(sorted_moves[0].get_src_square()) == 7 && static_cast<int>(sorted_moves[0].get_dest_square()) == 55);
// }

// void test_iterative_deepening_opponent_mate_in_1() {
//   // Positions from: https://lichess.org/study/IPtfJlNl
//   Magics::init();
//   Position pos = Position("4r2k/1p3rbp/2p1N1p1/p3n3/P2NB1nq/1P6/4R1P1/B1Q2RK1 b - - 4 32");
//   Opponent opp;
//   Search search = Search(false, opp);
//   Timer timer;
//   PositionStack ps;
//   Transposition_Table tt;
//   tt.resize(16);
//   u64 wtime = 60'000; // white has x msec left on the clock
//   u64 btime = 60'000; // black has x msec left on the clock
//   u64 winc = 0; // white increment per move in mseconds if x > 0
//   u64 binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search.iterative_deepening(pos, timer, tt, ps);
//   Move expected_move = Move(31, 15, Move_Flag::QUEEN_FLAG);
//   std::cout << Utils::move_to_board_notation(search.get_root_best_move()) << std::endl;
//   assert(search.get_root_best_move().equals(expected_move));
//   std::cout << "Test suite: test_negamax_iterative_deepening_opponent_mate_in_1: Test 1 passed\n";

//   pos = Position("4rb2/3qrk2/1p1p1n2/7p/P2P4/4R2P/1BQN1P2/1K4R1 w - - 3 39");
//   Search search2 = Search(false, opp);
//   timer = Timer();
//   ps = PositionStack();
//   tt = Transposition_Table();
//   tt.resize(16);
//   wtime = 60'000; // white has x msec left on the clock
//   btime = 60'000; // black has x msec left on the clock
//   winc = 0; // white increment per move in mseconds if x > 0
//   binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search2.iterative_deepening(pos, timer, tt, ps);
//   expected_move = Move(10, 46, Move_Flag::QUEEN_FLAG);
//   std::cout << Utils::move_to_board_notation(search2.get_root_best_move()) << std::endl;
//   assert(search2.get_root_best_move().equals(expected_move));
//   std::cout << "Test suite: test_negamax_iterative_deepening_opponent_mate_in_1: Test 2 passed\n";

//   pos = Position("r1bqkbnr/pp2pp1p/3p2p1/2p5/3nP3/2N3PP/PPP1NP2/R1BQKB1R b KQkq - 1 6");
//   Search search3 = Search(false, opp);
//   timer = Timer();
//   ps = PositionStack();
//   tt = Transposition_Table();
//   tt.resize(16);
//   wtime = 60'000; // white has x msec left on the clock
//   btime = 60'000; // black has x msec left on the clock
//   winc = 0; // white increment per move in mseconds if x > 0
//   binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search3.iterative_deepening(pos, timer, tt, ps);
//   ps = PositionStack();
//   tt = Transposition_Table();
//   tt.resize(16);
//   expected_move = Move(27, 21, Move_Flag::KNIGHT_FLAG);
//   std::cout << Utils::move_to_board_notation(search3.get_root_best_move()) << std::endl;
//   assert(search3.get_root_best_move().equals(expected_move));
//   std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_1: Test 3 passed\n";

//   pos = Position("r2q1rk1/1bppb1pp/n3p3/5P2/2BP1P2/4P3/1PPnQK1P/R1B3NR b - - 1 12");
//   Search search4 = Search(false, opp);
//   timer = Timer();
//   ps = PositionStack();
//   tt = Transposition_Table();
//   tt.resize(16);
//   wtime = 60'000; // white has x msec left on the clock
//   btime = 60'000; // black has x msec left on the clock
//   winc = 0; // white increment per move in mseconds if x > 0
//   binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search4.iterative_deepening(pos, timer, tt, ps);
//   expected_move = Move(52, 31, Move_Flag::BISHOP_FLAG);
//   std::cout << Utils::move_to_board_notation(search4.get_root_best_move()) << std::endl;
//   assert(search4.get_root_best_move().equals(expected_move));
//   std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_1: Test 4 passed\n";

//   pos = Position("3q1rk1/1bp3pp/r3P3/3p4/3PnP1b/1P2P3/2PNQ2P/R1BK2NR b - - 0 15");
//   Search search5 = Search(false, opp);
//   timer = Timer();
//   ps = PositionStack();
//   tt = Transposition_Table();
//   tt.resize(16);
//   wtime = 60'000; // white has x msec left on the clock
//   btime = 60'000; // black has x msec left on the clock
//   winc = 0; // white increment per move in mseconds if x > 0
//   binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search5.iterative_deepening(pos, timer, tt, ps);
//   expected_move = Move(28, 18, Move_Flag::KNIGHT_FLAG);
//   std::cout << Utils::move_to_board_notation(search5.get_root_best_move()) << std::endl;
//   assert(search5.get_root_best_move().equals(expected_move));
//   std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_1: Test 5 passed\n";
// }

// void test_iterative_deepening_opponent_mate_in_2() {
//   Magics::init();
//   Position pos = Position("4r3/1pp2rbk/6pn/4n3/P3BN1q/1PB2bPP/8/2Q1RRK1 b - - 0 31");
//   Opponent opp;
//   Search search = Search(false, opp);
//   Timer timer;
//   PositionStack ps;
//   Transposition_Table tt;
//   tt.resize(16);
//   u64 wtime = 60'000; // white has x msec left on the clock
//   u64 btime = 60'000; // black has x msec left on the clock
//   u64 winc = 0; // white increment per move in mseconds if x > 0
//   u64 binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search.iterative_deepening(pos, timer, tt, ps);
//   Move expected_move = Move(31, 22, Move_Flag::QUEEN_FLAG);
//   assert(search.get_root_best_move().equals(expected_move));
//   std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_2: Test 1 passed\n";

//   pos = Position("r2k1b1r/p1ppq2p/np3np1/5p2/1PPP4/P3PQ2/3N1PPP/R1B1K2R w KQ - 1 13");
//   Search search2 = Search(false, opp);
//   timer = Timer();
//   ps = PositionStack();
//   tt = Transposition_Table();
// tt.resize(16);
//   wtime = 60'000; // white has x msec left on the clock
//   btime = 60'000; // black has x msec left on the clock
//   winc = 0; // white increment per move in mseconds if x > 0
//   binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search2.iterative_deepening(pos, timer, tt, ps);
//   expected_move = Move(21, 56, Move_Flag::QUEEN_FLAG);
//   assert(search2.get_root_best_move().equals(expected_move));
//   std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_2: Test 2 passed\n";

//   pos = Position("rn3r2/p2q1pBk/1p2p3/3pP3/P1bNnQ2/5NP1/1P3PBP/R3b1K1 w - - 1 19");
//   Search search3 = Search(false, opp);
//   timer = Timer();
//   ps = PositionStack();
//   tt = Transposition_Table();
//   tt.resize(16);
//   wtime = 60'000; // white has x msec left on the clock
//   btime = 60'000; // black has x msec left on the clock
//   winc = 0; // white increment per move in mseconds if x > 0
//   binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search3.iterative_deepening(pos, timer, tt, ps);
//   expected_move = Move(29, 47, Move_Flag::QUEEN_FLAG);
//   assert(search3.get_root_best_move().equals(expected_move));
//   std::cout << "Test suite: test_negamax_fixed_depth_2_opponent_mate_in_2: Test 3 passed\n";
// }

// TEST(ITERATIVE_DEEPENING_NEGAMAX, opponent_mate_in_x) {
//   test_iterative_deepening_opponent_mate_in_1();
//   test_iterative_deepening_opponent_mate_in_2();
// }

// TEST(PERFT, testing_zobrist_debugger) {
//   Position pos = Position("k7/8/8/3p4/8/8/2n2K2/8 b - - 0 1");
//   Move move1 = Move(10, 4, Move_Flag::KNIGHT_FLAG);
//   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
//   pos.make_move(move1);
//   pos.make_move(move2);
//   Position new_pos = pos;
//   new_pos.recompute_zobrist_key();
//   std::cout << pos.equals_with_debugging(new_pos);
//   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
//   assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
// }

// TEST(ZOBRIST, zobrist1) {
// // Use cutechess and grab one of the outputs of position startpos moves ...
// // apply these moves to start pos and see if it equals the new position, using the FEN from cutechess (may need to plug into lichess to get FEN)
//   Position pos = Position();
//   Move move = Move(1, 16, Move_Flag::KNIGHT_FLAG);
//   Position new_pos = Position("rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 1 1");
//   pos.make_move(move); //calls to zobrist
//   std::cout << pos.equals_with_debugging(new_pos);
//   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
//   assert(pos.get_zobrist_key() == new_pos.get_zobrist_key());
// }

// TEST(ZOBRIST, zobrist3) {

//   Position pos = Position();
//   //Position new_position = Position("r4r2/p2q1pk1/3p2pp/2p5/4P3/5R2/PP4PP/5RK1 w - - 0 29");
//   Position new_position = Position("r2q1rk1/2pp1pbp/b1n1pnp1/pp6/8/N4N2/PPPPPPPP/1RBQKB1R w K - 2 10");
//   std::vector<std::string> moves = {"b1a3", "g7g6", "a1b1", "f8g7", "b1a1", "e7e6", "a1b1", "g8f6", "b1a1", "e8g8", "a1b1", "b8c6", "b1a1", "b7b5", "g1f3", "a7a5", "a1b1", "c8a6"};
//   for(std::string move_string : moves)
//   {
//     Move move = pos.board_notation_to_move(move_string);
//     pos.make_move(move);
//   }

//   std::cout << pos.equals_with_debugging(new_position);
//   std::cout << pos.zobrist_equals_with_debugging(new_position.get_zobrist_key());
//   assert(pos.equals(new_position));
//   assert(pos.get_zobrist_key() == new_position.get_zobrist_key());
// }

// TEST(ZOBRIST, zobrist4) {
//   Move move1 = Move(1, 18, Move_Flag::KNIGHT_FLAG);
//   Move move2 = Move(51, 43, Move_Flag::PAWN_FLAG);

//   Move move3 = Move(13, 21, Move_Flag::PAWN_FLAG);
//   Move move4 = Move(58, 37, Move_Flag::BISHOP_FLAG);

//   Move move5 = Move(12, 28, Move_Flag::PAWN_TWO_FORWARD_FLAG);

//   std::vector<Move> moves = {move1, move2, move3, move4, move5};

//   Position pos = Position();

//   // Ply 1:
//   Position posply1 = Position("rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR b KQkq - 1 1");
//   pos.make_move(move1);
//   assert(pos.equals(posply1));
//   assert(pos.get_zobrist_key() == posply1.get_zobrist_key());

//   // Ply 2:
//   Position posply2 = Position("rnbqkbnr/ppp1pppp/3p4/8/8/2N5/PPPPPPPP/R1BQKBNR w KQkq - 0 2");
//   pos.make_move(move2);
//   assert(pos.equals(posply2));
//   assert(pos.get_zobrist_key() == posply2.get_zobrist_key());

//   // Ply 3:
//   Position posply3 = Position("rnbqkbnr/ppp1pppp/3p4/8/8/2N2P2/PPPPP1PP/R1BQKBNR b KQkq - 0 2");
//   pos.make_move(move3);
//   assert(pos.equals(posply3));
//   assert(pos.get_zobrist_key() == posply3.get_zobrist_key());

//   // Ply 4:
//   Position posply4 = Position("rn1qkbnr/ppp1pppp/3p4/5b2/8/2N2P2/PPPPP1PP/R1BQKBNR w KQkq - 1 3");
//   pos.make_move(move4);
//   assert(pos.equals(posply4));
//   assert(pos.get_zobrist_key() == posply4.get_zobrist_key());

//   // Ply 5:
//   // Position posply5 = Position("rn1qkbnr/ppp1pppp/3p4/5b2/4P3/2N2P2/PPPP2PP/R1BQKBNR b KQkq - 0 3")
//   // en passant isnt in fen so had to manually update it:
//   Position posply5 = Position("rn1qkbnr/ppp1pppp/3p4/5b2/4P3/2N2P2/PPPP2PP/R1BQKBNR b KQkq e3 0 3");
//   pos.make_move(move5);
//   // std::cout << pos.equals_with_debugging(posply5);
//   // std::cout << pos.zobrist_equals_with_debugging(posply5.get_zobrist_key());
//   assert(pos.equals(posply5));
//   assert(pos.get_zobrist_key() == posply5.get_zobrist_key());
// }

// TEST(ZOBRIST, zobrist_recomputation) {
//   Move move1 = Move(1, 18, Move_Flag::KNIGHT_FLAG);
//   Move move2 = Move(51, 43, Move_Flag::PAWN_FLAG);

//   Move move3 = Move(13, 21, Move_Flag::PAWN_FLAG);
//   Move move4 = Move(58, 37, Move_Flag::BISHOP_FLAG);

//   Move move5 = Move(12, 28, Move_Flag::PAWN_TWO_FORWARD_FLAG);

//   std::vector<Move> moves = {move1, move2, move3, move4, move5};

//   Position pos = Position();

//   // Ply 1:
//   pos.make_move(move1);
//   Position posply1 = pos;
//   posply1.recompute_zobrist_key();
//   assert(pos.equals(posply1));
//   assert(pos.get_zobrist_key() == posply1.get_zobrist_key());

//   // Ply 2:
//   pos.make_move(move2);
//   Position posply2 = pos;
//   posply2.recompute_zobrist_key();
//   assert(pos.equals(posply2));
//   assert(pos.get_zobrist_key() == posply2.get_zobrist_key());

//   // Ply 3:
//   pos.make_move(move3);
//   Position posply3 = pos;
//   posply3.recompute_zobrist_key();
//   assert(pos.equals(posply3));
//   assert(pos.get_zobrist_key() == posply3.get_zobrist_key());

//   // Ply 4:
//   pos.make_move(move4);
//   Position posply4 = pos;
//   posply4.recompute_zobrist_key();
//   assert(pos.equals(posply4));
//   assert(pos.get_zobrist_key() == posply4.get_zobrist_key());

//   // Ply 5:
//   pos.make_move(move5);
//   Position posply5 = pos;
//   posply5.recompute_zobrist_key();
//   // std::cout << pos.equals_with_debugging(posply5);
//   // std::cout << pos.zobrist_equals_with_debugging(posply5.get_zobrist_key());
//   assert(pos.equals(posply5));
//   assert(pos.get_zobrist_key() == posply5.get_zobrist_key());
// }

// TEST(PERFT, another_zobrist1) {
//   Position pos = Position("k7/8/8/3p4/8/8/2n2K2/8 b - - 0 1");
//   Move move1 = Move(10, 4, Move_Flag::KNIGHT_FLAG);
//   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
//   pos.make_move(move1);
//   pos.make_move(move2);
//   Position new_pos = pos;
//   new_pos.recompute_zobrist_key();
//   std::cout << pos.equals_with_debugging(new_pos);
//   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
//   assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
// }

// TEST(PERFT, another_zobrist2) {
//   Position pos = Position("k7/3p4/8/8/8/8/5K2/4n3 b - - 0 1");
//   Move move1 = Move(51, 51-16, Move_Flag::PAWN_TWO_FORWARD_FLAG);
//   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
//   pos.make_move(move1);
//   pos.make_move(move2);
//   Position new_pos = pos;
//   new_pos.recompute_zobrist_key();
//   std::cout << pos.equals_with_debugging(new_pos);
//   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
//   assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
// }

// TEST(PERFT, another_zobrist3) {
//   Position pos = Position("1k6/8/8/3p4/8/8/5K2/4n3 b - - 0 1");
//   Move move1 = Move(57, 56, Move_Flag::KING_FLAG);
//   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
//   pos.make_move(move1);
//   pos.make_move(move2);
//   Position new_pos = pos;
//   new_pos.recompute_zobrist_key();
//   std::cout << pos.equals_with_debugging(new_pos);
//   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
//   assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
// }

// // TEST(PERFT, another_zobrist4) {
// //   Position pos = Position("k7/8/8/3p4/8/8/4pK2/8 b - - 0 1");
// //   Move move1 = Move(12, 4, Move_Flag::QUEEN_PROMOTION_FLAG);
// //   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
// //   pos.make_move(move1);
// //   pos.make_move(move2);
// //   Position new_pos = pos;
// //   new_pos.recompute_zobrist_key();
// //   std::cout << pos.equals_with_debugging(new_pos);
// //   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
// //   assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
// // }

// TEST(PERFT, another_zobrist5_capture) {
//   Position pos = Position("1k6/8/8/3p4/8/8/2n2K2/4R3 b - - 0 1");
//   Move move1 = Move(10, 4, Move_Flag::KNIGHT_FLAG);
//   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
//   pos.make_move(move1);
//   pos.make_move(move2);
//   Position new_pos = pos;
//   new_pos.recompute_zobrist_key();
//   std::cout << pos.equals_with_debugging(new_pos);
//   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
//   assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
// }

// TEST(PERFT, another_zobrist6) {
//   Position pos = Position("1k6/8/8/3p4/8/8/3p1K2/8 b - - 0 1");
//   Move move1 = Move(11, 3, Move_Flag::KNIGHT_PROMOTION_FLAG);
//   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
//   pos.make_move(move1);
//   pos.make_move(move2);
//   Position new_pos = pos;
//   new_pos.recompute_zobrist_key();
//     Utils::print_bb(pos.get_pawns());
//   Utils::print_bb(new_pos.get_pawns());
//   Utils::print_bb(pos.get_black_pieces());
//   Utils::print_bb(new_pos.get_black_pieces());
//   std::cout << pos.equals_with_debugging(new_pos);
//   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
//   assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
// }

// TEST(PERFT, another_zobrist7) {
//   Position pos = Position("1k6/8/8/3p4/8/8/p4K2/8 b - - 0 1");
//   Move move1 = Move(8, 0, Move_Flag::KNIGHT_PROMOTION_FLAG);
//   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
//   pos.make_move(move1);
//   pos.make_move(move2);
//   Position new_pos = pos;
//   new_pos.recompute_zobrist_key();
//     Utils::print_bb(pos.get_pawns());
//   Utils::print_bb(new_pos.get_pawns());
//   Utils::print_bb(pos.get_black_pieces());
//   Utils::print_bb(new_pos.get_black_pieces());
//   std::cout << pos.equals_with_debugging(new_pos);
//   std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
//   assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
// }


// TEST(PERFT, another_zobrist) {
//   Position pos = Position("k7/8/8/3p4/8/8/4pK2/8 b - - 0 1");
//   Move move1 = Move(12, 4, Move_Flag::KNIGHT_PROMOTION_FLAG);
//   Move move2 = Move(13, 12, Move_Flag::KING_FLAG);

//   // Testing whether after move 1, it breaks
//   pos.make_move(move1);
//   Position pos_ply1 = pos;
//   pos_ply1.recompute_zobrist_key();
//   std::cout << pos.equals_with_debugging(pos_ply1);
//   std::cout << pos.zobrist_equals_with_debugging(pos_ply1.get_zobrist_key());
//   assert(pos_ply1.get_zobrist_key() == pos.get_zobrist_key());

//   // Testing whether after move 2 on the intended position after move 1 indiviually breaks
//   Position pos_ply1_new = Position("k7/8/8/3p4/8/8/5K2/4n3 w - - 0 1");
//   pos_ply1_new.make_move(move2);
//   Position pos_ply2_new = pos_ply1_new;
//   pos_ply2_new.recompute_zobrist_key();
//   std::cout << pos_ply1_new.equals_with_debugging(pos_ply2_new);
//   std::cout << pos_ply1_new.zobrist_equals_with_debugging(pos_ply2_new.get_zobrist_key());
//   assert(pos_ply1_new.get_zobrist_key() == pos_ply2_new.get_zobrist_key());

//   // Diagnosis: after playing the moves in a row, it breaks.  Therefore, there is a problem within make move
//   pos.make_move(move2);
//   Position pos_ply2 = pos;
//   pos_ply2.recompute_zobrist_key();
//   std::cout << pos.equals_with_debugging(pos_ply2);
//   std::cout << pos.zobrist_equals_with_debugging(pos_ply2.get_zobrist_key());
//   assert(pos_ply2.get_zobrist_key() == pos.get_zobrist_key());
// }

// TEST(test, test) {
//   // Position pos = Position("rnbqkbnr/1ppppppp/8/p7/8/N7/PPPPPPPP/R1BQKBNR w KQkq - 0 2");
//   Move move = Move(0, 1, Move_Flag::ROOK_FLAG);
//   // pos.make_move(move);
//   // Position copy = pos;
//   // copy.recompute_zobrist_key();
//   // assert(pos.get_zobrist_key() == copy.get_zobrist_key());

//   // Move move2 = Move(57, 42, Move_Flag::KNIGHT_FLAG);
//   // pos.make_move(move2);
//   // Position copy_ = pos;
//   // copy_.recompute_zobrist_key();
//   // std::cout << pos.equals_with_debugging(copy_);
//   // std::cout << pos.zobrist_equals_with_debugging(copy_.get_zobrist_key());
//   // assert(copy_.get_zobrist_key() == pos.get_zobrist_key());

//   Position pos2 = Position("rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 1 1");
//   Zobrist::print_log();
//   Zobrist::clear_log();
//   std::cout << "\n\n\n";
//   Move move0 = Move(48, 32, Move_Flag::PAWN_TWO_FORWARD_FLAG);
//   pos2.make_move(move0);
//   Zobrist::print_log();
//   Zobrist::clear_log();
//   std::cout << "\n\n\n";
//   Position copy2 = pos2;
//   copy2.recompute_zobrist_key();
//   Zobrist::print_log();
//   Zobrist::clear_log();
//   std::cout << "\n\n\n";
//   std::cout << pos2.equals_with_debugging(copy2);
//   std::cout << pos2.zobrist_equals_with_debugging(copy2.get_zobrist_key());
//   assert(pos2.get_zobrist_key() == copy2.get_zobrist_key());

//   pos2.make_move(move);
//   Zobrist::print_log();
//   Zobrist::clear_log();
//   std::cout << "\n\n\n";
//   Position copy3 = pos2;
//   copy3.recompute_zobrist_key();
//   Zobrist::print_log();
//   Zobrist::clear_log();
//   std::cout << "\n\n\n";
//   std::cout << pos2.equals_with_debugging(copy3);
//   std::cout << pos2.zobrist_equals_with_debugging(copy3.get_zobrist_key());
//   assert(pos2.get_zobrist_key() == copy3.get_zobrist_key());
// }


// TEST(NEW_EN_PASSANT, one) {
//   Position pos = Position("rnbqkbnr/p1pppppp/8/8/Pp6/5N1P/1PPPPPP1/RNBQKB1R b KQkq a3 0 3");
//   std::cout << static_cast<int>(pos.get_en_passant_target()) << std::endl;
//   assert(static_cast<int>(pos.get_en_passant_target()) == 16);
// }

// TEST(MOVE_REPETITION, test_3_fold) {
//   Position pos = Position();
//   Move move1 = Move(1, 16, KNIGHT_FLAG);
//   Move move2 = Move(57, 40, KNIGHT_FLAG);
//   Move move3 = Move(16, 1, KNIGHT_FLAG);
//   Move move4 = Move(40, 57, KNIGHT_FLAG);
//   std::vector<u64> game_history_stack;
//   pos.make_move(move1);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move2);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move3);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move4);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move1);
//   game_history_stack.push_back(pos.get_zobrist_key()); // first repetition
//   pos.make_move(move2);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move3);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move4);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move1);
//   game_history_stack.push_back(pos.get_zobrist_key()); // second repetition
//   pos.make_move(move2);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move3);
//   game_history_stack.push_back(pos.get_zobrist_key());
//   pos.make_move(move4);
//   game_history_stack.push_back(pos.get_zobrist_key());

//   //Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 8 5");
//   Opponent opp;
//   Search search = Search(false, opp);
//   Timer timer;
//   PositionStack ps;
//   Transposition_Table tt;
//   tt.resize(16);
//   u64 wtime = 60'000; // white has x msec left on the clock
//   u64 btime = 60'000; // black has x msec left on the clock
//   u64 winc = 0; // white increment per move in mseconds if x > 0
//   u64 binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search.iterative_deepening(pos, timer, tt, ps);
//   // Should make_move on the same move as move1 at some point during search (in our case should be first as its the first move generated)
//   // Should detect a 3 move repetition, 2 moves before root and 1 at root + make_move
//   // Expect to see "Repetition detected" in the terminal
// }

// // Inspired by: https://github.com/toanth/motors/blob/main/motors/src/search/chess.rs
// TEST(MOVE_REPETITION, testing_2_fold) {
//   // position fen 8/3Q4/2K5/k7/6P1/8/8/8 w - - 0 1 moves d7d4 a5a6 d4d6 a6a5 d6d4 a5a6 d4d7 a6a5
//   // Move outputted should not be Qd4
// }

// TEST(STALEMATE_TEST, test1) {
//   // position fen 5q1k/8/5K2/2BB4/8/3Q4/8/8 w - - 0 1
//   // Should not capture the queen
// }

// TEST(EVAL, eval) {
//   Position pos;
//   std::cout << Evaluation::evaluate(pos) << std::endl;
//   Position pos2 = Position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
//   std::cout << Evaluation::evaluate(pos2) << std::endl;
// }

// // // mate in one: position fen 4r2k/1p3rbp/2p1N1p1/p3n3/P2NB1nq/1P6/4R1P1/B1Q2RK1 b - - 4 32



// TEST(GAMBIT_FEATURES, approach0) {
//   // Create a position where there is a move that is better than the AB move, probably to do with a queen blunder.
//   // Is true when eval is better than AB in some world (but not every world).
//   // Promoting pawn is better in a world that black doesnt capture back with a rook.
//   // However AB move is taking bishop
//     std::cout << "\n\n I AM HERE \n\n";
//   Position pos = Position("4nr2/5k1P/8/8/8/8/1K6/1R1b4 w - - 0 1");
//       std::cout << "\n\n I AM HERE 2 \n\n";
//   Opponent opp;
//   Search search = Search(false, opp);
//   Timer timer;
//   PositionStack ps;
//   Transposition_Table tt;
//   tt.resize(16);
//   u64 wtime = 60'000; // white has x msec left on the clock
//   u64 btime = 60'000; // black has x msec left on the clock
//   u64 winc = 0; // white increment per move in mseconds if x > 0
//   u64 binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search.iterative_deepening(pos, timer, tt, ps);
//   Move best_move = search.get_root_best_move();
//   std::string best_move_str = Utils::move_to_board_notation(best_move);
//   std::cout << "bestmove ID: " << best_move_str << std::endl;

//   // search.approach1_initial_call(pos);
//   // best_move = search.get_root_best_move();
//   // best_move_str = Utils::move_to_board_notation(best_move);
//   // std::cout << "bestmove approach1: " << best_move_str << std::endl;
// }

// TEST(IS_GAMBIT, prediction)
// {
//   Position pos = Position("4nr2/5k1P/8/8/8/8/1K6/1R1b4 w - - 0 1");
//   Opponent opp;
//   Search search = Search(false, opp);
//   Timer timer;
//   PositionStack ps;
//   Transposition_Table tt;
//   tt.resize(16);
//   u64 wtime = 60'000; // white has x msec left on the clock
//   u64 btime = 60'000; // black has x msec left on the clock
//   u64 winc = 0; // white increment per move in mseconds if x > 0
//   u64 binc = 0; // black increment per move in mseconds if x > 0
//   if(pos.get_turn() == Turn::WHITE) {
//       timer.set_fields(wtime, winc);
//   }
//   else {
//       timer.set_fields(btime, binc);
//   }
//   timer.start_timer();
//   search.iterative_deepening(pos, timer, tt, ps);
//   Move best_move = search.get_root_best_move();
//   std::string best_move_str = Utils::move_to_board_notation(best_move);
//   std::cout << "bestmove: " << best_move_str << std::endl;
// }

// TEST(QSEARCH, noisy_move_gen)
// {
//   // Gotten from https://www.chessprogramming.org/Perft_Results
//   // Where expected noisy size is number of captures minus number of en passant moves
//   // Used for qsearch perft
//   Position pos;
//   std::vector<Move> moves = pos.generate_all_moves(false);
//   std::vector<Move> noisy_moves = pos.generate_all_moves(true);
//   std::cout << "Moves size: " << moves.size() << std::endl;
//   std::cout << "Noisy moves size: " << noisy_moves.size() << std::endl;
// }


TEST(PERFT, perft_suite)
{
  Perft::run_perft_suite(false, true);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  Zobrist z;
  Magics::init();
  return RUN_ALL_TESTS();
}