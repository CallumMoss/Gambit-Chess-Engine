#include <gtest/gtest.h>
#include <iostream>

#include "../Position.hpp"
#include "../utils.hpp"
#include "../Magics.hpp"
#include "../Search.hpp"
#include "Perft.cpp"

// // Testing getters and FEN parser.
// TEST(FEN_And_Pos_Rep, initial_position) {
//   Position pos = Position();
//   EXPECT_TRUE(pos.get_wscr()) << "Expected white to have short castling rights, but it did not.";
//   EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
//   EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
//   EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

//   EXPECT_EQ(pos.get_pawns(), 0xff00000000ff00ULL) << "Pawns are not in the expected position.";
// 	EXPECT_EQ(pos.get_knights(), 0x4200000000000042ULL) << "Knights are not in the expected position.";
// 	EXPECT_EQ(pos.get_bishops(), 0x2400000000000024ULL) << "Bishops are not in the expected position.";
// 	EXPECT_EQ(pos.get_rooks(), 0x8100000000000081ULL) << "Rooks are not in the expected position.";
// 	EXPECT_EQ(pos.get_queens(), 0x800000000000008ULL) << "Queens are not in the expected position.";
// 	EXPECT_EQ(pos.get_kings(), 0x1000000000000010ULL) << "Kings are not in the expected position.";

//   EXPECT_EQ(pos.get_white_pawns(), 0xff00ULL) << "The white pawns are not in the expected position.";
// 	EXPECT_EQ(pos.get_black_pawns(), 0xff000000000000ULL) << "The black pawns are not in the expected position.";
//   EXPECT_EQ(pos.get_white_knights(), 0x42ULL) << "The white knights are not in the expected position.";
// 	EXPECT_EQ(pos.get_black_knights(), 0x4200000000000000ULL) << "The black knights are not in the expected position.";
//   EXPECT_EQ(pos.get_white_bishops(), 0x24ULL) << "The white bishops are not in the expected position.";
// 	EXPECT_EQ(pos.get_black_bishops(), 0x2400000000000000ULL) << "The black bishops are not in the expected position.";
//   EXPECT_EQ(pos.get_white_rooks(), 0x81ULL) << "The white rooks are not in the expected position.";
// 	EXPECT_EQ(pos.get_black_rooks(), 0x8100000000000000ULL) << "The black rooks are not in the expected position.";
//   EXPECT_EQ(pos.get_white_queen(), 0x8ULL) << "The white queen is not in the expected position.";
// 	EXPECT_EQ(pos.get_black_queen(), 0x800000000000000ULL) << "The black queen is not in the expected position.";
//   EXPECT_EQ(pos.get_white_king(), 0x10ULL) << "The white king is not in the expected position.";
// 	EXPECT_EQ(pos.get_black_king(), 0x1000000000000000ULL) << "The black king is not in the expected position.";

//   EXPECT_EQ(pos.get_white_pieces(), 0xffffULL) << "The white pieces are not in the expected position.";
// 	EXPECT_EQ(pos.get_black_pieces(), 0xffff000000000000ULL) << "The black pieces are not in the expected position.";
//   EXPECT_EQ(pos.get_board(), 0xffff00000000ffffULL) << "The pieces are not in the expected position.";

// 	EXPECT_EQ(pos.get_turn(), Turn::WHITE) << "Expected it to be white's turn, but it is not.";
// 	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
// 	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
// 	EXPECT_EQ(pos.get_full_move_counter(), 1) << "Expected the full move clock to be 1, but it is not.";
// }

// // We now know that getters work for getting specific coloured pieces. So only need to test pieces and colours indiviually.
// TEST(FEN_And_Pos_Rep, typical_middle_game) {
//   Position pos = Position("r3k2r/2pb1ppp/2pp1q2/p7/1nP1B3/1P2P3/P2N1PPP/R2QK2R w KQkq - 0 14");
//   EXPECT_TRUE(pos.get_wscr()) << "Expected white to have short castling rights, but it did not.";
//   EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
//   EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
//   EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

//   EXPECT_EQ(pos.get_pawns(), 0xe40c010412e100ULL) << "Pawns are not in the expected position.";
// 	EXPECT_EQ(pos.get_knights(), 0x2000800ULL) << "Knights are not in the expected position.";
// 	EXPECT_EQ(pos.get_bishops(), 0x8000010000000ULL) << "Bishops are not in the expected position.";
// 	EXPECT_EQ(pos.get_rooks(), 0x8100000000000081ULL) << "Rooks are not in the expected position.";
// 	EXPECT_EQ(pos.get_queens(), 0x200000000008ULL) << "Queens are not in the expected position.";
// 	EXPECT_EQ(pos.get_kings(), 0x1000000000000010ULL) << "Kings are not in the expected position.";

//   EXPECT_EQ(pos.get_white_pieces(), 0x1412e999ULL) << "The white pieces are not in the expected position.";
// 	EXPECT_EQ(pos.get_black_pieces(), 0x91ec2c0102000000ULL) << "The black pieces are not in the expected position.";

// 	EXPECT_EQ(pos.get_turn(), Turn::WHITE) << "Expected it to be white's turn, but it is not.";
// 	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
// 	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
// 	EXPECT_EQ(pos.get_full_move_counter(), 14) << "Expected the full move clock to be 14, but it is not.";
// }

// // Testing slightly middle game with a half move, blacks turn and white having castled.
// TEST(FEN_And_Pos_Rep, middle_game_white_castled) {
//   Position pos = Position("r1bqk2r/ppp2ppp/2n1pn2/3p4/3P4/2NBPN2/PPPB1PPP/R2Q1RK1 b kq - 1 7");
//   EXPECT_FALSE(pos.get_wscr()) << "Expected white to not have short castling rights, but it did.";
//   EXPECT_FALSE(pos.get_wlcr()) << "Expected white to not have long castling rights, but it did.";
//   EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
//   EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

//   EXPECT_EQ(pos.get_pawns(), 0xe710080810e700ULL) << "Pawns are not in the expected position.";
// 	EXPECT_EQ(pos.get_knights(), 0x240000240000ULL) << "Knights are not in the expected position.";
// 	EXPECT_EQ(pos.get_bishops(), 0x400000000080800ULL) << "Bishops are not in the expected position.";
// 	EXPECT_EQ(pos.get_rooks(), 0x8100000000000021ULL) << "Rooks are not in the expected position.";
// 	EXPECT_EQ(pos.get_queens(), 0x800000000000008ULL) << "Queens are not in the expected position.";
// 	EXPECT_EQ(pos.get_kings(), 0x1000000000000040ULL) << "Kings are not in the expected position.";

//   EXPECT_EQ(pos.get_white_pieces(), 0x83cef69ULL) << "The white pieces are not in the expected position.";
// 	EXPECT_EQ(pos.get_black_pieces(), 0x9de7340800000000ULL) << "The black pieces are not in the expected position.";

// 	EXPECT_EQ(pos.get_turn(), Turn::BLACK) << "Expected it to be white's turn, but it is not.";
// 	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
// 	EXPECT_EQ(pos.get_half_move_clock(), 1) << "Expected the half move clock to be 1, but it is not.";
// 	EXPECT_EQ(pos.get_full_move_counter(), 7) << "Expected the full move clock to be 7, but it is not.";
// }

// // Tests en passant, blacks turn and white castling after moving rook
// TEST(FEN_And_Pos_Rep, complex_position) {
//   Position pos = Position("2kr3r/pb1pppbp/1pn5/q1p1n3/2BP2pP/PPN1BN2/2PQ1PP1/R3KR2 b Q h3 0 14");
//   EXPECT_FALSE(pos.get_wscr()) << "Expected white to not have short castling rights, but it did.";
//   EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
//   EXPECT_FALSE(pos.get_bscr()) << "Expected black to have short castling rights, but it did.";
//   EXPECT_FALSE(pos.get_blcr()) << "Expected black to have long castling rights, but it did.";

//   EXPECT_EQ(pos.get_pawns(), 0xb90204c8036400ULL) << "Pawns are not in the expected position.";
// 	EXPECT_EQ(pos.get_knights(), 0x41000240000ULL) << "Knights are not in the expected position.";
// 	EXPECT_EQ(pos.get_bishops(), 0x42000004100000ULL) << "Bishops are not in the expected position.";
// 	EXPECT_EQ(pos.get_rooks(), 0x8800000000000021ULL) << "Rooks are not in the expected position.";
// 	EXPECT_EQ(pos.get_queens(), 0x100000800ULL) << "Queens are not in the expected position.";
// 	EXPECT_EQ(pos.get_kings(), 0x400000000000010ULL) << "Kings are not in the expected position.";

//   EXPECT_EQ(pos.get_white_pieces(), 0x8c376c31ULL) << "The white pieces are not in the expected position.";
// 	EXPECT_EQ(pos.get_black_pieces(), 0x8cfb061540000000ULL) << "The black pieces are not in the expected position.";

// 	EXPECT_EQ(pos.get_turn(), Turn::BLACK) << "Expected it to be white's turn, but it is not.";
// 	EXPECT_EQ(pos.get_en_passant_target(), 0x800000ULL) << "Expected no en passant posibility, but there is.";
// 	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
// 	EXPECT_EQ(pos.get_full_move_counter(), 14) << "Expected the full move clock to be 14, but it is not.";
// }

// Manually reviewing precomputations (passes regardless)
// TEST(PRECOMPUTATION_VALIDATION, all_pieces_validation) {
// 	for(int i = 0; i < 64; i++) {
// 		Utils::PrintBB(Utils::ROOK_ATTACKS[i], i, true);
// 	}
// 	for(int i = 0; i < 64; i++) {
// 		Utils::PrintBB(Utils::BISHOP_ATTACKS[i], i, true);
// 	}
// 	for(int i = 0; i < 64; i++) {
// 		Utils::PrintBB(Utils::KNIGHT_ATTACKS[i], i, true);
// 	}
// 	for(int i = 0; i < 64; i++) {
// 		Utils::PrintBB(Utils::QUEEN_ATTACKS[i], i, true);
// 	}
// 	for(int i = 0; i < 64; i++) {
// 		Utils::PrintBB(Utils::KING_ATTACKS[i], i, true);
// 	}
// }
// Manually reviewing precomputations (passes regardless)
// TEST(PRECOMPUTATION_VALIDATION, knight_move_extraction) {
// 	Position pos = Position();
// 	int i = 30;
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::KNIGHT, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing precomputations (passes regardless)
// TEST(PRECOMPUTATION_VALIDATION, bishop_move_extraction) {
// 	Position pos = Position();
// 	int i = 30;
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::BISHOP, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing precomputations (passes regardless)
// TEST(PRECOMPUTATION_VALIDATION, rook_move_extraction) {
// 	Position pos = Position("8/8/8/8/6R1/8/8/8 w - - 0 1");
// 	int i = 30;
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::ROOK, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing precomputations (passes regardless)
// TEST(PRECOMPUTATION_VALIDATION, queen_move_extraction) {
// 	Position pos = Position();
// 	int i = 30;
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::QUEEN, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing precomputations (passes regardless)
// TEST(PRECOMPUTATION_VALIDATION, king_move_extraction) {
// 	Position pos = Position();
// 	int i = 30;
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::KING, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing pawn moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, pawns) {
// 	Position pos = Position();
// 	int i = 12;
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::PAWN, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing pawn moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, pawns_diagonal) {
// 	Position pos = Position();
// 	int i = 12 + (4*8);
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::PAWN, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing pawn moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, pawns_black) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::BLACK);
// 	int i = 12 + (5*8);
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::PAWN, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing pawn moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, pawns_black_diagonal) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::BLACK);
// 	int i = 20;
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::PAWN, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // Manually reviewing pawn moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, pawns_forward_double) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	int i = 20;
// 	std::vector<u64> moves = pos.generate_piece_moves(Piece::PAWN, i);
// 	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
// 		Utils::PrintBB(moves[q], i, true);
// 	}
// }

// // // Manually reviewing rook moves (passes regardless)
// // TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers) {
// // 	Position pos = Position();
// // 	pos.set_turn(Turn::WHITE);
// // 	u64 moves;
// // 	for(int i = 0; i < 64; i++) {
// // 		moves = pos.pseudo_legalise_rook_attacks(i, 0);
// // 		Utils::PrintBB(moves, i, true);
// // 	}
// // }

// // // Manually reviewing rook moves (passes regardless)
// // TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers_complex) {
// // 	Position pos = Position("2kr3r/pb1pppbp/1pn5/q1p1n3/2BP2pP/PPN1BN2/2PQ1PP1/R3KR2 b Q h3 0 14");
// // 	pos.set_turn(Turn::WHITE);
// // 	u64 moves;
// // 	int i = 0;
// // 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// // 	Utils::PrintBB(moves, i, true);

// // 	i = 5;
// // 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// // 	Utils::PrintBB(moves, i, true);

// // 	i = 59;
// // 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// // 	Utils::PrintBB(moves, i, true);

// // 	i = 63;
// // 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// // 	Utils::PrintBB(moves, i, true);
// // }

// // // Manually reviewing rook moves (passes regardless)
// // TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers_complex2) {
// // 	Position pos = Position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
// // 	pos.set_turn(Turn::WHITE);
// // 	u64 moves;
// // 	int i = 25;
// // 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// // 	Utils::PrintBB(moves, i, true);

// // 	i = 39;
// // 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// // 	Utils::PrintBB(moves, i, true);
// // } 

// // // Manually reviewing rook moves (passes regardless)
// // TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers) {
// // 	Position pos = Position();
// // 	pos.set_turn(Turn::WHITE);
// // 	Utils::PrintBB(pos.get_blockers(20), 20, true);

// // 	// bb_vector moves;
// // 	// for(int i = 0; i < 64; i++) {
// // 	// 	moves = pos.pseudo_legalise_rook_attacks(0, Utils::ROOK_ATTACKS[0]);
// // 	// 	Utils::PrintBB(moves[i], 0, true);
// // 	// }
// // }

// // // Manually reviewing rook moves (passes regardless)
// // TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers) {
// // 	Position pos = Position();
// // 	pos.set_turn(Turn::WHITE);
// // 	// Utils::PrintBB(pos.pseudo_legalise_rook_attacks_slow(20, Utils::ROOK_ATTACKS[20]), 20, true);

// // 	bb_vector moves;
// // 	for(int i = 0; i < 64; i++) {
// // 		Utils::PrintBB(pos.pseudo_legalise_rook_attacks_slow(i, Utils::ROOK_ATTACKS[i]), i, true);
// // 	}
// // }

// // Manually reviewing bishop moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, bishop_no_edges_precalculations) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	for(int i = 0; i < 64; i++) {
// 		Utils::PrintBB(Utils::BISHOP_ATTACKS_NO_EDGES[i], i, true);
// 	}
// }

// // Manually reviewing bishop moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, queen_precalculations) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	for(int i = 0; i < 64; i++) {
// 		u64 queen_attacks = Utils::BISHOP_ATTACKS[i] | Utils::ROOK_ATTACKS[i];
// 		std::cout << queen_attacks << std::endl;
// 		Utils::PrintBB(queen_attacks, i, true);
// 	}
// }

// // Manually reviewing bishop moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, queen_no_edges_precalculations) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	for(int i = 0; i < 64; i++) {
// 		u64 queen_attacks = Utils::BISHOP_ATTACKS_NO_EDGES[i] | Utils::ROOK_ATTACKS_NO_EDGES[i];
// 		std::cout << queen_attacks << std::endl;
// 		//Utils::PrintBB(queen_attacks, i, true);
// 	}
// }

// // Manually reviewing rook moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, rook_no_edges_precalculations) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	for(int i = 0; i < 64; i++) {
// 		Utils::PrintBB(Utils::ROOK_ATTACKS_NO_EDGES[i], i, true);
// 	}
// }

// // Manually reviewing rook moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, finding_magic) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	int square = 0;
// 	u64 blocker_squares = Magics::get_relevant_blocker_squares(Piece::ROOK, square);
// 	Utils::PrintBB(blocker_squares, square);
// 	Final_Magic magic = Magics::find_magic(Piece::ROOK, square);
// 	std::cout << "Done";
// 	std::cout << "\nMagic Number: " << magic.magic.magic_number << "\n";
// 	// for(size_t i = 0; i < magic.table.size(); i++) {
// 	// 	std::cout << magic.table[i] << "\n ########################################### \n";
// 	// }
// }

// Tests finding of magics
// TEST(GEN_MOVES_VALIDATION, find_magic) {
// 	// 8/3N4/1p3P2/8/3B4/5p2/5P2/8 w - - 0 1 Bishop testing position
// 	Position pos = Position("8/3P4/5p2/8/2pR4/5P2/8/3p4 w - - 0 1");
// 	pos.set_turn(Turn::WHITE);
// 	int i = 27;
// 	Final_Magic fm = Magics::find_magic(Piece::ROOK, i);
// 	u64 attacks = fm.table[Magics::get_magic_index(fm.magic, Magics::get_blockers(i, pos.get_board()))];
// 	Utils::PrintBB(pos.get_board(), i, true);
// 	Utils::PrintBB(Utils::ROOK_ATTACKS[i], i, true);
// 	Utils::PrintBB(Utils::ROOK_ATTACKS_NO_EDGES[i], i, true);
// 	Utils::PrintBB(Magics::get_blockers(i, pos.get_board()), i, true);
// 	Utils::PrintBB(attacks, i, true);
// }

// ARCHIVED:
// Tests finding of magics
// TEST(GEN_MOVES_VALIDATION, getting_magics) {
// 	// I used this function to get the magics:
// 	Position pos = Position();
// 	Magics::init();
// 	for(Final_Magic magic : rook_magics_table) {
// 		std::cout << magic.magic.magic_number << std::endl;
// 	}
// 	std::cout << "\n ################### \n";
// 	for(Final_Magic magic : bishop_magics_table) {
// 		std::cout << magic.magic.magic_number << std::endl;
// 	}
	
// }

// TEST(GEN_MOVES_VALIDATION, using_generated_magics) {
// 	Position pos = Position();
// 	Magics::init();
// 	int square = 0;
// 	bb_vector attack_table_for_square = rook_magics_table[square];
// 	// for(int i = 0; i < (int)attack_table_for_square.size(); i++) {
// 	// 	Utils::PrintBB(attack_table_for_square[i], square, true);
// 	// }
// }

// TEST(GEN_MOVES_VALIDATION, using_generated_magics) {
// 	// For an empty board
// 	Position pos = Position();
// 	Magics::init(pos);
// 	int square = 0;
// 	bb_vector attack_table_for_square = rook_magics_table[square];
// 	for(int i = 0; i < (int)attack_table_for_square.size(); i++) {
// 		std::cout << i << std::endl;
// 		Utils::PrintBB(attack_table_for_square[i], square, true);
// 	}
// }

// TEST(GEN_MOVES_VALIDATION, using_generated_magics2)
// {
// 	Position pos = Position("8/3P4/5p2/8/2pR1P2/5P2/8/3p4 w - - 0 1");
// 	Magics::init();
// 	int square = 27;
// 	std::cout << "Hey\n";
// 	u64 rook_moves = pos.get_rook_moves(pos, square);
// 	Utils::PrintBB(rook_moves, square, true);
// }

// TEST(GEN_MOVES_VALIDATION, using_generated_magics3)
// {
// 	Position pos = Position("8/3P4/5p2/8/2pR1P2/5P2/8/3p4 w - - 0 1");
// 	Magics::init();
// 	int square = 27;
// 	bb_vector rook_moves = pos.generate_piece_moves(Piece::ROOK, square);
// 	for(u64 rook_move : rook_moves) {
// 		Utils::PrintBB(rook_move, square, true);
// 	}
// }

// // Manually reviewing bishop moves (passes regardless)
// // TEST(GEN_MOVES_VALIDATION, bishop_slow_move_gen_no_blockers) {
// // 	Position pos = Position();
// // 	pos.set_turn(Turn::WHITE);
// // 	u64 moves;
// // 	for(int i = 0; i < 64; i++) {
// // 		moves = Magics::pseudo_legalise_bishop_attacks_slow(i, 0);
// // 		Utils::PrintBB(moves, i, true);
// // 	}
// // }

// // // Manually reviewing bishop moves (passes regardless)
// // TEST(GEN_MOVES_VALIDATION, bishop_slow_move_gen_blockers) {
// // 	Position pos = Position();
// // 	pos.set_turn(Turn::WHITE);
// // 	u64 moves;
// // 	for(int i = 0; i < 64; i++) {
// // 		moves = Magics::pseudo_legalise_bishop_attacks_slow(i, Magics::get_blockers(Piece::BISHOP, i, pos.get_board()));
// // 		Utils::PrintBB(moves, i, true);
// // 	}
// // }

// // // Manually reviewing bishop moves (passes regardless)
// // TEST(GEN_MOVES_VALIDATION, bishop_find_magics) {
// // 	Position pos = Position();
// // 	Magics::init();
// // 	for(Final_Magic magic : bishop_magics_table) {
// // 		std::cout << magic.magic.magic_number << std::endl;
// // 	}
// // }

// // // Tests finding of magics
// // TEST(GEN_MOVES_VALIDATION, testing_move_gen) {
// // 	Position pos = Position("8/3P4/5p2/8/2pR1P2/5P2/8/3p4 w - - 0 1");
// // 	Magics::init();
// // 	int square = 27;
// // 	bb_vector moves = pos.generate_piece_moves(Piece::ROOK, square);
// // 	for(u64 move : moves) {
// // 		Utils::PrintBB(move, square, true);
// // 	}
// // }

// // Tests use of magics
// TEST(GEN_MOVES_VALIDATION, testing_move_gen2) {
// 	Position pos = Position("8/p2P4/5p2/4P3/3B4/4pP2/8/3p4 w - - 0 1");
// 	Magics::init();
// 	int square = 27;
// 	bb_vector moves = pos.generate_piece_moves(Piece::BISHOP, square);
// 	for(u64 move : moves) {
// 		Utils::PrintBB(move, square, true);
// 	}
// }

// // Tests use of magics
// TEST(GEN_MOVES_VALIDATION, testing_move_gen3) {
// 	Position pos = Position("8/p2P4/5p2/1p2P3/2pQ2P1/4pP2/8/3p4 w - - 0 1");
// 	pos.set_turn(Turn::BLACK);
// 	Magics::init();
// 	int square = 27;
// 	bb_vector moves = pos.generate_piece_moves(Piece::QUEEN, square);
// 	for(u64 move : moves) {
// 		Utils::PrintBB(move, square, true);
// 	}
// }

// // Tests use of magics
// TEST(GEN_MOVES_VALIDATION, testing_black_bishop) {
// 	Position pos = Position("8/p2P4/5p2/1p2P3/2pb2P1/4pP2/8/3p4 w - - 0 1");
// 	pos.set_turn(Turn::BLACK);
// 	Magics::init();
// 	int square = 27;
// 	bb_vector moves = pos.generate_piece_moves(Piece::BISHOP, square);
// 	for(u64 move : moves) {
// 		Utils::PrintBB(move, square, true);
// 	}
// }

// // Tests use of magics
// TEST(GEN_MOVES_VALIDATION, testing_black_rook) {
// 	Position pos = Position("8/p2P4/5p2/1p2P3/2pr2P1/4pP2/8/3p4 w - - 0 1");
// 	pos.set_turn(Turn::BLACK);
// 	Magics::init();
// 	int square = 27;
// 	bb_vector moves = pos.generate_piece_moves(Piece::ROOK, square);
// 	for(u64 move : moves) {
// 		Utils::PrintBB(move, square, true);
// 	}
// }

// // Tests use of magics
// TEST(GEN_MOVES_VALIDATION, testing_black_queen) {
// 	Position pos = Position("8/p2P4/5p2/1p2P3/2pq2P1/4pP2/8/3p4 w - - 0 1");
// 	pos.set_turn(Turn::BLACK);
// 	Magics::init();
// 	int square = 27;
// 	bb_vector moves = pos.generate_piece_moves(Piece::QUEEN, square);
// 	for(u64 move : moves) {
// 		Utils::PrintBB(move, square, true);
// 	}
// }








// TEST(MAKE_MOVES, testing_forward_pawn_push) {
// 	Position pos = Position();
// 	u8 src_square = 12;
// 	u8 dest_square = 20;
// 	Move move = Move(src_square, dest_square, Move_Flag::PAWN_FLAG);
// 	pos.copy_make(move, pos);
// 	Utils::PrintBB(pos.get_board(), src_square, true);
// 	Utils::PrintBB(pos.get_pawns(), src_square, true);
// 	Utils::PrintBB(pos.get_white_pawns(), src_square, true);
// 	Utils::PrintBB(pos.get_white_pieces(), src_square, true);
// }

// TEST(MAKE_MOVES, testing_forward_two_pawn_push) {
// 	Position pos = Position();
// 	u8 src_square = 12;
// 	u8 dest_square = 28;
// 	Move move = Move(src_square, dest_square, Move_Flag::PAWN_TWO_FORWARD_FLAG);
// 	pos.copy_make(move, pos);
// 	Utils::PrintBB(pos.get_board(), src_square, true);
// 	Utils::PrintBB(pos.get_pawns(), src_square, true);
// 	Utils::PrintBB(pos.get_white_pawns(), src_square, true);
// 	Utils::PrintBB(pos.get_white_pieces(), src_square, true);
// }

// TEST(SQUARE_CONVERSION, x1) {
// 	for(int i = 0; i < 64; i++) {
// 		std::cout << Utils::index_to_board_notation(i) << std::endl;
// 	}
// }

// TEST(PERFT, pos1_ply1) { // where ply is a half move, in this case whites opening move
// 	Position pos = Position();
// 	Magics::init();
// 	int depth = 4;
// 	std::cout << "Position 1: (Depth 1)" << std::endl << pos.split_perft(depth, depth) << std::endl;
// }



// To debug using perft and web perft:
// Find up to a depth where you are accurate
// then for the first failed depth, compare the difference of your results
// Go down the line of moves with incorrect nodes till you get to the original moves that were wrong at depth 1
// Solve the problem

// TEST(PERFT, pos1) { // starting position
// 	Position pos = Position();
// 	Magics::init();
// 	int depth = 6; // where depth is measured in half moves / plys
// 	std::cout << "Position 1: (Depth " << depth << ")" << std::endl << pos.split_perft(depth, depth) << std::endl;
// }

// TEST(PERFT, pos2) { // conventionally named kiwipete
// 	Position pos = Position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
// 	Magics::init();
// 	int depth = 5; // where depth is measured in half moves / plys
// 	std::cout << "Position 2: (Depth " << depth << ")" << std::endl << pos.split_perft(depth, depth) << std::endl;
// }

// TEST(PERFT, pos3) {
// 	Position pos = Position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
// 	Magics::init();
// 	int depth = 7; // where depth is measured in half moves / plys
// 	std::cout << "Position 3: (Depth " << depth << ")" << std::endl << pos.split_perft(depth, depth) << std::endl;
// }

// TEST(PERFT, pos4) {
// 	Position pos = Position("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
// 	Magics::init();
// 	int depth = 5; // where depth is measured in half moves / plys
// 	std::cout << "Position 4: (Depth " << depth << ")" << std::endl << pos.split_perft(depth, depth) << std::endl;
// }

// TEST(PERFT, pos5) {
// 	Position pos = Position("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
// 	Magics::init();
// 	int depth = 5; // where depth is measured in half moves / plys
// 	std::cout << "Position 5: (Depth " << depth << ")" << std::endl << pos.split_perft(depth, depth) << std::endl;
// }

// TEST(PERFT, pos6) {
// 	Position pos = Position("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
// 	Magics::init();
// 	int depth = 5; // where depth is measured in half moves / plys
// 	std::cout << "Position 6: (Depth " << depth << ")" << std::endl << pos.split_perft(depth, depth) << std::endl;
// }

TEST(PERFT, perft_suite) {
	Magics::init();
	ASSERT_TRUE(run_perft_suite(false));
}

// TEST(SEARCH, mvv_lva) {
//   Piece victim = Piece::PAWN;
//   Piece attacker = Piece::QUEEN;
// 	std::cout << Utils::find_mvv_lva(victim, attacker) << std::endl; // 45
// }

// TEST(SEARCH, mvv_lva2) {
//   Piece victim = Piece::QUEEN;
//   Piece attacker = Piece::PAWN;
// 	std::cout << Utils::find_mvv_lva(victim, attacker) << std::endl; // 9
// }

// TEST(SEARCH, mvv_lva3) {
//   Piece victim = Piece::KING;
//   Piece attacker = Piece::PAWN;
// 	ASSERT_EXIT(
//     { std::cerr << Utils::find_mvv_lva(victim, attacker) << std::endl; },
//     ::testing::ExitedWithCode(-1),                                        
//     "Unexpected piece type of victim: 5"                   
//   );
// }

// TEST(SEARCH, mvv_lva4) { // lowest
//   Piece victim = Piece::QUEEN;
//   Piece attacker = Piece::KING;
// 	std::cout << Utils::find_mvv_lva(victim, attacker) << std::endl; // 4
// }

// TEST(SEARCH, mvv_lva5) { // highest
//   Piece victim = Piece::PAWN;
//   Piece attacker = Piece::PAWN;
// 	std::cout << Utils::find_mvv_lva(victim, attacker) << std::endl; // 49
// }

// TEST(SEARCH, mvv_lva_sorting) { // testing whether sorted returns the correct number of moves
//   Position pos = Position("5k2/7p/6p1/8/8/8/8/3K3R w - - 0 1");
//   Magics::init();
//   std::vector<Move> moves = pos.generate_all_moves();
//   std::vector<Move> sorted_moves = Utils::sort_by_mvv_lva(moves, pos);
//   Piece victim = Piece::PAWN;
//   Piece attacker = Piece::ROOK;
//   assert(Utils::find_mvv_lva(victim, attacker) == 46);
//   assert(moves.size() == sorted_moves.size());
//   //std::cout << Utils::move_to_board_notation(sorted_moves[static_cast<int>(sorted_moves.size()) - 1]) << std::endl;
//   assert(static_cast<int>(sorted_moves[0].get_src_square()) == 7 && static_cast<int>(sorted_moves[0].get_dest_square()) == 55);
// }

TEST(FIXED_DEPTH_NEGAMAX, mate_in_1) {
  Position pos = Position("4r2k/1p3rbp/2p1N1p1/p3n3/P2NB1nq/1P6/4R1P1/B1Q2RK1 b - - 4 32");
  Search search = Search();
  int depth = 2;
  search.negamax2(depth, 0, pos);
  std::cout << Utils::move_to_board_notation(search.get_root_best_move()) << std::endl;
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}