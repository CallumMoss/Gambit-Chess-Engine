#include "Evaluation.hpp"

int Evaluation::evaluate(const Position& pos) {
    int white_material = count_material(pos, Turn::WHITE);
    int black_material = count_material(pos, Turn::BLACK);
    bool is_endgame = false;
    if(white_material + black_material < 2200) { // if either side has a total worth of a queen and two pawns each, endgame starts
        is_endgame = true;
    }
    int eval = white_material - black_material;
    eval += calculate_piece_square_advantage(pos, Turn::WHITE, is_endgame); // check values from piece square tables
    eval -= calculate_piece_square_advantage(pos, Turn::BLACK, is_endgame);
    if(pos.get_turn() == Turn::WHITE) { return eval; }
    else { return -eval; } 
}

int Evaluation::count_material(const Position& pos, Turn colour) {
    int material = 0;
    u64 board;
    if(colour == Turn::WHITE) {
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

int Evaluation::calculate_piece_square_advantage(const Position& pos, Turn colour, bool is_endgame) {
    int piece_square_advantage = 0;
    u64 board;
    if(colour == Turn::WHITE) {
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
                if(colour == Turn::WHITE) { piece_square_advantage += Evaluation::WHITE_PAWN_SQUARE_TABLE[square]; }
                else { piece_square_advantage += Evaluation::BLACK_PAWN_SQUARE_TABLE[square]; }
                break;
            case(Piece::KNIGHT):
                piece_square_advantage += Evaluation::KNIGHT_SQUARE_TABLE[square];
                break;
            case(Piece::BISHOP):
                piece_square_advantage += Evaluation::BISHOP_SQUARE_TABLE[square];
                break;
            case(Piece::ROOK):
                if(colour == Turn::WHITE) { piece_square_advantage += Evaluation::WHITE_ROOK_SQUARE_TABLE[square]; }
                else { piece_square_advantage += Evaluation::BLACK_ROOK_SQUARE_TABLE[square]; }
                break;
            case(Piece::QUEEN):
                piece_square_advantage += Evaluation::QUEEN_SQUARE_TABLE[square];
                break;
            case(Piece::KING):
                if(is_endgame) {
                    if(colour == Turn::WHITE) { piece_square_advantage += Evaluation::WHITE_KING_SQUARE_TABLE_END_GAME[square]; }
                    else { piece_square_advantage += Evaluation::BLACK_KING_SQUARE_TABLE_END_GAME[square]; }
                }
                else {
                    if(colour == Turn::WHITE) { piece_square_advantage += Evaluation::WHITE_KING_SQUARE_TABLE[square]; }
                    else { piece_square_advantage += Evaluation::BLACK_KING_SQUARE_TABLE[square]; }
                }
            default:
                break;
        }
        board = Utils::clear_bit(board, square);
    }
    return piece_square_advantage;
}

bool Evaluation::is_endgame(const Position& pos) {
    if(pos.get_queens() == 0ULL) {
        return true; // if queens are off the board
    }
    return false;
}