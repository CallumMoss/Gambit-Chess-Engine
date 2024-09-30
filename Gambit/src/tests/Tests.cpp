#include <gtest/gtest.h>
#include "../Position.hpp"
#include "../utils.hpp"
#include <iostream>

// Testing getters and FEN parser.
TEST(FEN_And_Pos_Rep, initial_position) {
  Position pos = Position();
  EXPECT_TRUE(pos.get_wscr()) << "Expected white to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
  EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

  EXPECT_EQ(pos.get_pawns(), 0xff00000000ff00ULL) << "Pawns are not in the expected position.";
	EXPECT_EQ(pos.get_knights(), 0x4200000000000042ULL) << "Knights are not in the expected position.";
	EXPECT_EQ(pos.get_bishops(), 0x2400000000000024ULL) << "Bishops are not in the expected position.";
	EXPECT_EQ(pos.get_rooks(), 0x8100000000000081ULL) << "Rooks are not in the expected position.";
	EXPECT_EQ(pos.get_queens(), 0x800000000000008ULL) << "Queens are not in the expected position.";
	EXPECT_EQ(pos.get_kings(), 0x1000000000000010ULL) << "Kings are not in the expected position.";

  EXPECT_EQ(pos.get_white_pawns(), 0xff00ULL) << "The white pawns are not in the expected position.";
	EXPECT_EQ(pos.get_black_pawns(), 0xff000000000000ULL) << "The black pawns are not in the expected position.";
  EXPECT_EQ(pos.get_white_knights(), 0x42ULL) << "The white knights are not in the expected position.";
	EXPECT_EQ(pos.get_black_knights(), 0x4200000000000000ULL) << "The black knights are not in the expected position.";
  EXPECT_EQ(pos.get_white_bishops(), 0x24ULL) << "The white bishops are not in the expected position.";
	EXPECT_EQ(pos.get_black_bishops(), 0x2400000000000000ULL) << "The black bishops are not in the expected position.";
  EXPECT_EQ(pos.get_white_rooks(), 0x81ULL) << "The white rooks are not in the expected position.";
	EXPECT_EQ(pos.get_black_rooks(), 0x8100000000000000ULL) << "The black rooks are not in the expected position.";
  EXPECT_EQ(pos.get_white_queen(), 0x8ULL) << "The white queen is not in the expected position.";
	EXPECT_EQ(pos.get_black_queen(), 0x800000000000000ULL) << "The black queen is not in the expected position.";
  EXPECT_EQ(pos.get_white_king(), 0x10ULL) << "The white king is not in the expected position.";
	EXPECT_EQ(pos.get_black_king(), 0x1000000000000000ULL) << "The black king is not in the expected position.";

  EXPECT_EQ(pos.get_white_pieces(), 0xffffULL) << "The white pieces are not in the expected position.";
	EXPECT_EQ(pos.get_black_pieces(), 0xffff000000000000ULL) << "The black pieces are not in the expected position.";
  EXPECT_EQ(pos.get_board(), 0xffff00000000ffffULL) << "The pieces are not in the expected position.";

	EXPECT_EQ(pos.get_turn(), Turn::WHITE) << "Expected it to be white's turn, but it is not.";
	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
	EXPECT_EQ(pos.get_full_move_counter(), 1) << "Expected the full move clock to be 1, but it is not.";
}

// We now know that getters work for getting specific coloured pieces. So only need to test pieces and colours indiviually.
TEST(FEN_And_Pos_Rep, typical_middle_game) {
  Position pos = Position("r3k2r/2pb1ppp/2pp1q2/p7/1nP1B3/1P2P3/P2N1PPP/R2QK2R w KQkq - 0 14");
  EXPECT_TRUE(pos.get_wscr()) << "Expected white to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
  EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

  EXPECT_EQ(pos.get_pawns(), 0xe40c010412e100ULL) << "Pawns are not in the expected position.";
	EXPECT_EQ(pos.get_knights(), 0x2000800ULL) << "Knights are not in the expected position.";
	EXPECT_EQ(pos.get_bishops(), 0x8000010000000ULL) << "Bishops are not in the expected position.";
	EXPECT_EQ(pos.get_rooks(), 0x8100000000000081ULL) << "Rooks are not in the expected position.";
	EXPECT_EQ(pos.get_queens(), 0x200000000008ULL) << "Queens are not in the expected position.";
	EXPECT_EQ(pos.get_kings(), 0x1000000000000010ULL) << "Kings are not in the expected position.";

  EXPECT_EQ(pos.get_white_pieces(), 0x1412e999ULL) << "The white pieces are not in the expected position.";
	EXPECT_EQ(pos.get_black_pieces(), 0x91ec2c0102000000ULL) << "The black pieces are not in the expected position.";

	EXPECT_EQ(pos.get_turn(), Turn::WHITE) << "Expected it to be white's turn, but it is not.";
	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
	EXPECT_EQ(pos.get_full_move_counter(), 14) << "Expected the full move clock to be 14, but it is not.";
}

// Testing slightly middle game with a half move, blacks turn and white having castled.
TEST(FEN_And_Pos_Rep, middle_game_white_castled) {
  Position pos = Position("r1bqk2r/ppp2ppp/2n1pn2/3p4/3P4/2NBPN2/PPPB1PPP/R2Q1RK1 b kq - 1 7");
  EXPECT_FALSE(pos.get_wscr()) << "Expected white to not have short castling rights, but it did.";
  EXPECT_FALSE(pos.get_wlcr()) << "Expected white to not have long castling rights, but it did.";
  EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

  EXPECT_EQ(pos.get_pawns(), 0xe710080810e700ULL) << "Pawns are not in the expected position.";
	EXPECT_EQ(pos.get_knights(), 0x240000240000ULL) << "Knights are not in the expected position.";
	EXPECT_EQ(pos.get_bishops(), 0x400000000080800ULL) << "Bishops are not in the expected position.";
	EXPECT_EQ(pos.get_rooks(), 0x8100000000000021ULL) << "Rooks are not in the expected position.";
	EXPECT_EQ(pos.get_queens(), 0x800000000000008ULL) << "Queens are not in the expected position.";
	EXPECT_EQ(pos.get_kings(), 0x1000000000000040ULL) << "Kings are not in the expected position.";

  EXPECT_EQ(pos.get_white_pieces(), 0x83cef69ULL) << "The white pieces are not in the expected position.";
	EXPECT_EQ(pos.get_black_pieces(), 0x9de7340800000000ULL) << "The black pieces are not in the expected position.";

	EXPECT_EQ(pos.get_turn(), Turn::BLACK) << "Expected it to be white's turn, but it is not.";
	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
	EXPECT_EQ(pos.get_half_move_clock(), 1) << "Expected the half move clock to be 1, but it is not.";
	EXPECT_EQ(pos.get_full_move_counter(), 7) << "Expected the full move clock to be 7, but it is not.";
}

// Tests en passant, blacks turn and white castling after moving rook
TEST(FEN_And_Pos_Rep, complex_position) {
  Position pos = Position("2kr3r/pb1pppbp/1pn5/q1p1n3/2BP2pP/PPN1BN2/2PQ1PP1/R3KR2 b Q h3 0 14");
  EXPECT_FALSE(pos.get_wscr()) << "Expected white to not have short castling rights, but it did.";
  EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
  EXPECT_FALSE(pos.get_bscr()) << "Expected black to have short castling rights, but it did.";
  EXPECT_FALSE(pos.get_blcr()) << "Expected black to have long castling rights, but it did.";

  EXPECT_EQ(pos.get_pawns(), 0xb90204c8036400ULL) << "Pawns are not in the expected position.";
	EXPECT_EQ(pos.get_knights(), 0x41000240000ULL) << "Knights are not in the expected position.";
	EXPECT_EQ(pos.get_bishops(), 0x42000004100000ULL) << "Bishops are not in the expected position.";
	EXPECT_EQ(pos.get_rooks(), 0x8800000000000021ULL) << "Rooks are not in the expected position.";
	EXPECT_EQ(pos.get_queens(), 0x100000800ULL) << "Queens are not in the expected position.";
	EXPECT_EQ(pos.get_kings(), 0x400000000000010ULL) << "Kings are not in the expected position.";

  EXPECT_EQ(pos.get_white_pieces(), 0x8c376c31ULL) << "The white pieces are not in the expected position.";
	EXPECT_EQ(pos.get_black_pieces(), 0x8cfb061540000000ULL) << "The black pieces are not in the expected position.";

	EXPECT_EQ(pos.get_turn(), Turn::BLACK) << "Expected it to be white's turn, but it is not.";
	EXPECT_EQ(pos.get_en_passant_target(), 0x800000ULL) << "Expected no en passant posibility, but there is.";
	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
	EXPECT_EQ(pos.get_full_move_counter(), 14) << "Expected the full move clock to be 14, but it is not.";
}

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
TEST(PRECOMPUTATION_VALIDATION, knight_move_extraction) {
	Position pos = Position();
	int i = 30;
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::KNIGHT);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing precomputations (passes regardless)
TEST(PRECOMPUTATION_VALIDATION, bishop_move_extraction) {
	Position pos = Position();
	int i = 30;
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::BISHOP);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing precomputations (passes regardless)
TEST(PRECOMPUTATION_VALIDATION, rook_move_extraction) {
	Position pos = Position();
	int i = 30;
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::ROOK);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing precomputations (passes regardless)
TEST(PRECOMPUTATION_VALIDATION, queen_move_extraction) {
	Position pos = Position();
	int i = 30;
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::QUEEN);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing precomputations (passes regardless)
TEST(PRECOMPUTATION_VALIDATION, king_move_extraction) {
	Position pos = Position();
	int i = 30;
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::KING);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing pawn moves (passes regardless)
TEST(GEN_MOVES_VALIDATION, pawns) {
	Position pos = Position();
	int i = 12;
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::PAWN);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing pawn moves (passes regardless)
TEST(GEN_MOVES_VALIDATION, pawns_diagonal) {
	Position pos = Position();
	int i = 12 + (4*8);
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::PAWN);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing pawn moves (passes regardless)
TEST(GEN_MOVES_VALIDATION, pawns_black) {
	Position pos = Position();
	pos.set_turn(Turn::BLACK);
	int i = 12 + (5*8);
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::PAWN);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing pawn moves (passes regardless)
TEST(GEN_MOVES_VALIDATION, pawns_black_diagonal) {
	Position pos = Position();
	pos.set_turn(Turn::BLACK);
	int i = 20;
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::PAWN);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// Manually reviewing pawn moves (passes regardless)
TEST(GEN_MOVES_VALIDATION, pawns_forward_double) {
	Position pos = Position();
	pos.set_turn(Turn::WHITE);
	int i = 20;
	std::vector<u64> moves = pos.generate_piece_moves(i, Piece::PAWN);
	for(std::size_t q = 0; q < moves.size(); q++) { // uses std::size_t because thats the return type of size
		Utils::PrintBB(moves[q], i, true);
	}
}

// // Manually reviewing rook moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	u64 moves;
// 	for(int i = 0; i < 64; i++) {
// 		moves = pos.pseudo_legalise_rook_attacks(i, 0);
// 		Utils::PrintBB(moves, i, true);
// 	}
// }

// // Manually reviewing rook moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers_complex) {
// 	Position pos = Position("2kr3r/pb1pppbp/1pn5/q1p1n3/2BP2pP/PPN1BN2/2PQ1PP1/R3KR2 b Q h3 0 14");
// 	pos.set_turn(Turn::WHITE);
// 	u64 moves;
// 	int i = 0;
// 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// 	Utils::PrintBB(moves, i, true);

// 	i = 5;
// 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// 	Utils::PrintBB(moves, i, true);

// 	i = 59;
// 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// 	Utils::PrintBB(moves, i, true);

// 	i = 63;
// 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// 	Utils::PrintBB(moves, i, true);
// }

// // Manually reviewing rook moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers_complex2) {
// 	Position pos = Position("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
// 	pos.set_turn(Turn::WHITE);
// 	u64 moves;
// 	int i = 25;
// 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// 	Utils::PrintBB(moves, i, true);

// 	i = 39;
// 	moves = pos.pseudo_legalise_rook_attacks(i, 0);
// 	Utils::PrintBB(moves, i, true);
// } 

// // Manually reviewing rook moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	Utils::PrintBB(pos.get_blockers(20), 20, true);


// /////////////.];][kpoln get edge squares for rook on each square


// 	// bb_vector moves;
// 	// for(int i = 0; i < 64; i++) {
// 	// 	moves = pos.pseudo_legalise_rook_attacks(0, Utils::ROOK_ATTACKS[0]);
// 	// 	Utils::PrintBB(moves[i], 0, true);
// 	// }
// }

// // Manually reviewing rook moves (passes regardless)
// TEST(GEN_MOVES_VALIDATION, rook_relevant_blockers) {
// 	Position pos = Position();
// 	pos.set_turn(Turn::WHITE);
// 	// Utils::PrintBB(pos.pseudo_legalise_rook_attacks_slow(20, Utils::ROOK_ATTACKS[20]), 20, true);

// 	bb_vector moves;
// 	for(int i = 0; i < 64; i++) {
// 		Utils::PrintBB(pos.pseudo_legalise_rook_attacks_slow(i, Utils::ROOK_ATTACKS[i]), i, true);
// 	}
// }

// Manually reviewing rook moves (passes regardless)
TEST(GEN_MOVES_VALIDATION, rook_no_edges_precalculations) {
	Position pos = Position();
	pos.set_turn(Turn::WHITE);
	for(int i = 0; i < 64; i++) {
		Utils::PrintBB(Utils::ROOK_ATTACKS_NO_EDGES[i], i, true);
	}
}

// Manually reviewing rook moves (passes regardless)
TEST(GEN_MOVES_VALIDATION, finding_magic) {
	Position pos = Position();
	pos.set_turn(Turn::WHITE);
	int square = 0;
	u64 blocker_squares = pos.get_relevant_blocker_squares(square);
	Utils::PrintBB(blocker_squares, square, true);
	Final_Magic magic = pos.find_magic(square);
	std::cout << "\nMagic Number: " << magic.magic.magic_number << "\n";
	for(size_t i = 0; i < magic.table.size(); i++) {
		std::cout << magic.table[i] << "\n ########################################### \n";
	}
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}