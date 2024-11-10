// #include "Position.hpp"

// // Finds the best move for the current position using negamax
// // Initial call for negamax for each move up to a line depth provided
// Move Position::find_best_move(const Position& current_position, u8 depth) {
//     // Currently do not keep track of eval after end
//     // If wish to do this for programming analysis, can
//     // just use prints
//     int best_eval = INT_MIN;
//     Move best_move = Move(0, 0, Move_Flag::NULL_FLAG);
//     int eval;
//     // Need to convert this to return moves instead of bitboards
//     std::vector<Move> moves = generate_all_moves();
//     Position new_position;
//      for(Move move : moves) {
//         // Reset position
//         new_position = current_position;
        
//         // make move
//         copy_make(move, new_position); // apply move to current pos
//         if(!new_position.legality_check()) { // if move isnt legal, skip eval for this move
//             continue;
//         }

//         // depth - 1 because we have already looked at this depth
//         // eg: depth 1 = our move
//         // eg: depth 2 = opponents response
//         // uses depth of half moves

//         // Should figure out how to make use of turns
//         // in this case, it should be called with the turn that isnt ours
//         // for simplicity we assume we are white for now
//         // calls -negamax as to flip the eval back to our perspective

//         // AB Negamax:
//         eval = -negamax_ab(new_position, depth - 1, INT_MIN, INT_MAX);

//         if(eval > best_eval) {
//             best_eval = eval;
//             best_move = move;
//         }
//      }
//      return best_move;
// }

// // Inspired from the chess programming wiki (standard convention)
// // Search algorithm which produces the same output as minimax but is simpler to implement

// // Pruning is less helpful for gambit as it aims to find risky moves, therefore pruning could prove bad
// // Added pruning simply but may take it off for the above reason
// // negamax and negamax_ab returns the same best move, therefore chose to implement this version.
// int Position::negamax_ab(Position pos, u8 depth, int alpha, int beta) {
//     if (depth == 0) { // or if checkmate?
//         // returns eval of the position at a given depth
//         return evaluate(pos);
//     }

//     std::vector<Move> moves = pos.generate_all_moves();
//     int best_eval;
//     for(Move move : moves) {
//         pos.copy_make(move, pos);

//         int eval = -negamax_ab(pos, depth - 1, -beta, -alpha);
//         // do i need to undo the move? shouldnt need to if i copy positions instead
//         // of editing our one position

//         if(eval > best_eval) {
//             best_eval = eval;
//         }
//         if(eval > alpha) {
//             alpha = eval;

//             if(beta >= eval) {
//                 break;
//             }
//         }
//     }

//     return best_eval;

// }