#include "Evaluation.hpp"
#include "Position.hpp"

int Evaluation::evaluate(Position& pos) {
    int eval = count_material(pos, Colour::WHITE) - count_material(pos, Colour::BLACK);
    if(pos.get_turn() == Turn::WHITE) { return eval; }
    else { return -eval; } 
}

int Evaluation::count_material(Position& pos, Colour colour) {
    int material = 0;
    u64 board;
    if(colour == Colour::WHITE) {
        board = pos.get_white_pieces();
    }
    else {
        board = pos.get_black_pieces();
    }
    Piece piece;
    int square = 0;
    while(true) {
        // find ls1b square
        square = Utils::find_piece_index(board);
        if(square == 64) { // no more pieces
            break;
        }
        piece = pos.get_piece_type_from_square(square);
        switch(piece) {
            case(Piece::PAWN):
                material += Piece_Values::PAWN_VALUE;
                break;
            case(Piece::KNIGHT):
                material += Piece_Values::KNIGHT_VALUE;
                break;
            case(Piece::BISHOP):
                material += Piece_Values::BISHOP_VALUE;
                break;
            case(Piece::ROOK):
                material += Piece_Values::ROOK_VALUE;
                break;
            case(Piece::QUEEN):
                material += Piece_Values::QUEEN_VALUE;
                break;
            default: // Kings and Invalid have no value
                break;
        }
        board = Utils::clear_bit(board, square);
    }
    return material;
}