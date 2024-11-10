// #include "Position.hpp"

// int Position::evaluate(Position pos) {
//     // I think chess perspective of - for black is performed in another function, so dont need to consider anything besides own side - opp side
//     if(pos.get_turn() == Turn::WHITE) {
//         // could make a get_opp_turn()?
//         return count_material(Turn::WHITE) - count_material(Turn::BLACK);
//     }
//     else {
//         return count_material(Turn::BLACK) - count_material(Turn::WHITE);
//     } 
// }

// u8 Position::count_material(Turn turn) {
//     u8 material = 0;
//     u64 board;
//     if(turn == Turn::WHITE) {
//         board = get_white_pieces();
//     }
//     else {
//         board = get_black_pieces();
//     }
//     Piece piece;
//     while(true) {
//         piece = get_piece_type_from_square(Utils::find_piece_index(board));
//         switch(piece) {
//             case(Piece::PAWN):
//                 material += Piece_Values::PAWN_VALUE;
//                 break;
//             case(Piece::KNIGHT):
//                 material += Piece_Values::KNIGHT_VALUE;
//                 break;
//             case(Piece::BISHOP):
//                 material += Piece_Values::BISHOP_VALUE;
//                 break;
//             case(Piece::ROOK):
//                 material += Piece_Values::ROOK_VALUE;
//                 break;
//             case(Piece::QUEEN):
//                 material += Piece_Values::QUEEN_VALUE;
//                 break;
//             default: // Kings and Invalid have no value
//                 break;
//         }
//     }
//     return material;
// }