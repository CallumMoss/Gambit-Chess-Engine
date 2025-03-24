#ifndef ZOBRIST_TESTS_HPP
#define ZOBRIST_TESTS_HPP


#include "../Types.hpp"
#include "../Position.hpp"
#include "../utils.hpp"
#include "../Magics.hpp"
#include "../Search.hpp"
#include "../Opponent.hpp"

inline void test_zobrist_1()
{
    Position pos = Position("k7/8/8/3p4/8/8/2n2K2/8 b - - 0 1");
    Move move1 = Move(10, 4, Move_Flag::KNIGHT_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
    pos.make_move(move1);
    pos.make_move(move2);
    Position new_pos = pos;
    new_pos.recompute_zobrist_key();
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
}
  
inline void test_zobrist_2()
{
    // Use cutechess and grab one of the outputs of position startpos moves ...
    // apply these moves to start pos and see if it equals the new position, using the FEN from cutechess (may need to plug into lichess to get FEN)
    Position pos = Position();
    Move move = Move(1, 16, Move_Flag::KNIGHT_FLAG);
    Position new_pos = Position("rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 1 1");
    pos.make_move(move); //calls to zobrist
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(pos.get_zobrist_key() == new_pos.get_zobrist_key());
}
  
inline void test_zobrist_3()
{
    Position pos = Position();
    //Position new_position = Position("r4r2/p2q1pk1/3p2pp/2p5/4P3/5R2/PP4PP/5RK1 w - - 0 29");
    Position new_position = Position("r2q1rk1/2pp1pbp/b1n1pnp1/pp6/8/N4N2/PPPPPPPP/1RBQKB1R w K - 2 10");
    std::vector<std::string> moves = {"b1a3", "g7g6", "a1b1", "f8g7", "b1a1", "e7e6", "a1b1", "g8f6", "b1a1", "e8g8", "a1b1", "b8c6", "b1a1", "b7b5", "g1f3", "a7a5", "a1b1", "c8a6"};
    for(std::string move_string : moves)
    {
      Move move = pos.board_notation_to_move(move_string);
      pos.make_move(move);
    }
  
    std::cout << pos.equals_with_debugging(new_position);
    std::cout << pos.zobrist_equals_with_debugging(new_position.get_zobrist_key());
    assert(pos.equals(new_position));
    assert(pos.get_zobrist_key() == new_position.get_zobrist_key());
}
  
inline void test_zobrist_4()
{
    Move move1 = Move(1, 18, Move_Flag::KNIGHT_FLAG);
    Move move2 = Move(51, 43, Move_Flag::PAWN_FLAG);
  
    Move move3 = Move(13, 21, Move_Flag::PAWN_FLAG);
    Move move4 = Move(58, 37, Move_Flag::BISHOP_FLAG);
  
    Move move5 = Move(12, 28, Move_Flag::PAWN_TWO_FORWARD_FLAG);
  
    std::vector<Move> moves = {move1, move2, move3, move4, move5};
  
    Position pos = Position();
  
    // Ply 1:
    Position posply1 = Position("rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR b KQkq - 1 1");
    pos.make_move(move1);
    assert(pos.equals(posply1));
    assert(pos.get_zobrist_key() == posply1.get_zobrist_key());
  
    // Ply 2:
    Position posply2 = Position("rnbqkbnr/ppp1pppp/3p4/8/8/2N5/PPPPPPPP/R1BQKBNR w KQkq - 0 2");
    pos.make_move(move2);
    assert(pos.equals(posply2));
    assert(pos.get_zobrist_key() == posply2.get_zobrist_key());
  
    // Ply 3:
    Position posply3 = Position("rnbqkbnr/ppp1pppp/3p4/8/8/2N2P2/PPPPP1PP/R1BQKBNR b KQkq - 0 2");
    pos.make_move(move3);
    assert(pos.equals(posply3));
    assert(pos.get_zobrist_key() == posply3.get_zobrist_key());
  
    // Ply 4:
    Position posply4 = Position("rn1qkbnr/ppp1pppp/3p4/5b2/8/2N2P2/PPPPP1PP/R1BQKBNR w KQkq - 1 3");
    pos.make_move(move4);
    assert(pos.equals(posply4));
    assert(pos.get_zobrist_key() == posply4.get_zobrist_key());
  
    // Ply 5:
    // Position posply5 = Position("rn1qkbnr/ppp1pppp/3p4/5b2/4P3/2N2P2/PPPP2PP/R1BQKBNR b KQkq - 0 3")
    // en passant isnt in fen so had to manually update it:
    Position posply5 = Position("rn1qkbnr/ppp1pppp/3p4/5b2/4P3/2N2P2/PPPP2PP/R1BQKBNR b KQkq e3 0 3");
    pos.make_move(move5);
    // std::cout << pos.equals_with_debugging(posply5);
    // std::cout << pos.zobrist_equals_with_debugging(posply5.get_zobrist_key());
    assert(pos.equals(posply5));
    assert(pos.get_zobrist_key() == posply5.get_zobrist_key());
}
  
inline void test_zobrist_recomputation_1()
{
    Move move1 = Move(1, 18, Move_Flag::KNIGHT_FLAG);
    Move move2 = Move(51, 43, Move_Flag::PAWN_FLAG);
  
    Move move3 = Move(13, 21, Move_Flag::PAWN_FLAG);
    Move move4 = Move(58, 37, Move_Flag::BISHOP_FLAG);
  
    Move move5 = Move(12, 28, Move_Flag::PAWN_TWO_FORWARD_FLAG);
  
    std::vector<Move> moves = {move1, move2, move3, move4, move5};
  
    Position pos = Position();
  
    // Ply 1:
    pos.make_move(move1);
    Position posply1 = pos;
    posply1.recompute_zobrist_key();
    assert(pos.equals(posply1));
    assert(pos.get_zobrist_key() == posply1.get_zobrist_key());
  
    // Ply 2:
    pos.make_move(move2);
    Position posply2 = pos;
    posply2.recompute_zobrist_key();
    assert(pos.equals(posply2));
    assert(pos.get_zobrist_key() == posply2.get_zobrist_key());
  
    // Ply 3:
    pos.make_move(move3);
    Position posply3 = pos;
    posply3.recompute_zobrist_key();
    assert(pos.equals(posply3));
    assert(pos.get_zobrist_key() == posply3.get_zobrist_key());
  
    // Ply 4:
    pos.make_move(move4);
    Position posply4 = pos;
    posply4.recompute_zobrist_key();
    assert(pos.equals(posply4));
    assert(pos.get_zobrist_key() == posply4.get_zobrist_key());
  
    // Ply 5:
    pos.make_move(move5);
    Position posply5 = pos;
    posply5.recompute_zobrist_key();
    // std::cout << pos.equals_with_debugging(posply5);
    // std::cout << pos.zobrist_equals_with_debugging(posply5.get_zobrist_key());
    assert(pos.equals(posply5));
    assert(pos.get_zobrist_key() == posply5.get_zobrist_key());
}
  
inline void test_zobrist_recomputation_2()
{
    Position pos = Position("k7/8/8/3p4/8/8/2n2K2/8 b - - 0 1");
    Move move1 = Move(10, 4, Move_Flag::KNIGHT_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
    pos.make_move(move1);
    pos.make_move(move2);
    Position new_pos = pos;
    new_pos.recompute_zobrist_key();
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
}
  
  inline void test_zobrist_recomputation_3()
  {
    Position pos = Position("k7/3p4/8/8/8/8/5K2/4n3 b - - 0 1");
    Move move1 = Move(51, 51-16, Move_Flag::PAWN_TWO_FORWARD_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
    pos.make_move(move1);
    pos.make_move(move2);
    Position new_pos = pos;
    new_pos.recompute_zobrist_key();
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
}
  
inline void test_zobrist_recomputation_4()
{
    Position pos = Position("1k6/8/8/3p4/8/8/5K2/4n3 b - - 0 1");
    Move move1 = Move(57, 56, Move_Flag::KING_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
    pos.make_move(move1);
    pos.make_move(move2);
    Position new_pos = pos;
    new_pos.recompute_zobrist_key();
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
}
  
inline void test_zobrist_recomputation_5()
{
    Position pos = Position("k7/8/8/3p4/8/8/4pK2/8 b - - 0 1");
    Move move1 = Move(12, 4, Move_Flag::QUEEN_PROMOTION_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
    pos.make_move(move1);
    pos.make_move(move2);
    Position new_pos = pos;
    new_pos.recompute_zobrist_key();
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
}
  
inline void test_zobrist_recomputation_6()
{ // Piece capture
    Position pos = Position("1k6/8/8/3p4/8/8/2n2K2/4R3 b - - 0 1");
    Move move1 = Move(10, 4, Move_Flag::KNIGHT_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
    pos.make_move(move1);
    pos.make_move(move2);
    Position new_pos = pos;
    new_pos.recompute_zobrist_key();
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
}
  
inline void test_zobrist_recomputation_7()
{
    Position pos = Position("1k6/8/8/3p4/8/8/3p1K2/8 b - - 0 1");
    Move move1 = Move(11, 3, Move_Flag::KNIGHT_PROMOTION_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
    pos.make_move(move1);
    pos.make_move(move2);
    Position new_pos = pos;
    new_pos.recompute_zobrist_key();
      Utils::print_bb(pos.get_pawns());
    Utils::print_bb(new_pos.get_pawns());
    Utils::print_bb(pos.get_black_pieces());
    Utils::print_bb(new_pos.get_black_pieces());
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
}
  
inline void test_zobrist_recomputation_8()
{
    Position pos = Position("1k6/8/8/3p4/8/8/p4K2/8 b - - 0 1");
    Move move1 = Move(8, 0, Move_Flag::KNIGHT_PROMOTION_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
    pos.make_move(move1);
    pos.make_move(move2);
    Position new_pos = pos;
    new_pos.recompute_zobrist_key();
    Utils::print_bb(pos.get_pawns());
    Utils::print_bb(new_pos.get_pawns());
    Utils::print_bb(pos.get_black_pieces());
    Utils::print_bb(new_pos.get_black_pieces());
    std::cout << pos.equals_with_debugging(new_pos);
    std::cout << pos.zobrist_equals_with_debugging(new_pos.get_zobrist_key());
    assert(new_pos.get_zobrist_key() == pos.get_zobrist_key());
}
  
  
inline void test_zobrist_recomputation_9()
{
    Position pos = Position("k7/8/8/3p4/8/8/4pK2/8 b - - 0 1");
    Move move1 = Move(12, 4, Move_Flag::KNIGHT_PROMOTION_FLAG);
    Move move2 = Move(13, 12, Move_Flag::KING_FLAG);
  
    // Testing whether after move 1, it breaks
    pos.make_move(move1);
    Position pos_ply1 = pos;
    pos_ply1.recompute_zobrist_key();
    std::cout << pos.equals_with_debugging(pos_ply1);
    std::cout << pos.zobrist_equals_with_debugging(pos_ply1.get_zobrist_key());
    assert(pos_ply1.get_zobrist_key() == pos.get_zobrist_key());
  
    // Testing whether after move 2 on the intended position after move 1 indiviually breaks
    Position pos_ply1_new = Position("k7/8/8/3p4/8/8/5K2/4n3 w - - 0 1");
    pos_ply1_new.make_move(move2);
    Position pos_ply2_new = pos_ply1_new;
    pos_ply2_new.recompute_zobrist_key();
    std::cout << pos_ply1_new.equals_with_debugging(pos_ply2_new);
    std::cout << pos_ply1_new.zobrist_equals_with_debugging(pos_ply2_new.get_zobrist_key());
    assert(pos_ply1_new.get_zobrist_key() == pos_ply2_new.get_zobrist_key());
  
    // If breaks: Diagnosis: after playing the moves in a row, it breaks. Therefore, there is a problem within make move
    pos.make_move(move2);
    Position pos_ply2 = pos;
    pos_ply2.recompute_zobrist_key();
    std::cout << pos.equals_with_debugging(pos_ply2);
    std::cout << pos.zobrist_equals_with_debugging(pos_ply2.get_zobrist_key());
    assert(pos_ply2.get_zobrist_key() == pos.get_zobrist_key());
  }

#endif // ZOBRIST_TESTS_HPP