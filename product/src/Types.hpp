#ifndef TYPES_HPP
#define TYPES_HPP

// Define a macro "gambit debug" to replace anywhere with GAMBIT_DEBUG with the value to the right of it.
// 1 if want to check asserts, 0 for ignore
#define GAMBIT_DEBUG 1
#if GAMBIT_DEBUG == 1
#else
#define NDEBUG
#endif

#include <cassert>
#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <vector>
#include <list>

// Aliases:
using u8 = uint8_t;
using u16 = uint16_t;
using u64 = uint64_t;

// Types:
enum Piece
{
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
    INVALID = -1
};

enum Piece_Values
{
    // Valued in centipawns
    PAWN_VALUE = 100,
    KNIGHT_VALUE = 300, 
    BISHOP_VALUE = 300,
    ROOK_VALUE = 500,
    QUEEN_VALUE = 900,
};

enum Castling_Rights
{
    WHITE_SHORT = 0,
    WHITE_LONG = 1,
    BLACK_SHORT = 2,
    BLACK_LONG = 3
};

enum Turn
{
    WHITE = 0,
    BLACK = 1
};

constexpr Turn operator!(Turn t) { return Turn(int(t) ^ 1); }

struct MagicEntry {
    u64 mask;
    u64 magic_number;
    int index_bits; // Could probably use a u8 for smaller data type but this keeps it simple
};

// Inspired by Starzix flags:
// https://github.com/zzzzz151/Starzix/blob/main/src/move.hpp
enum Move_Flag: u8 {
    NULL_FLAG             = 0x00,
    PAWN_FLAG             = 0x01,
    KNIGHT_FLAG           = 0x02,
    BISHOP_FLAG           = 0x03,
    ROOK_FLAG             = 0x04,
    QUEEN_FLAG            = 0x05,
    KING_FLAG             = 0x06,

    // Only 1 flag can be used at a time, however the piece type can be inferred from any of these special flags
    PAWN_TWO_FORWARD_FLAG = 0x07,
    EN_PASSANT_FLAG       = 0x08,
    KNIGHT_PROMOTION_FLAG = 0x09,
    BISHOP_PROMOTION_FLAG = 0x0A,
    ROOK_PROMOTION_FLAG   = 0x0B,
    QUEEN_PROMOTION_FLAG  = 0x0C,
    CASTLING_FLAG         = 0x0D
};
struct Move {
    // 16 bits from right to left:
    // 4: move_flag
    // 6: dest square
    // 6: src square
    // EG: 000000 0000000 0000
    //     src    dest    flag
    u16 move = 0;

    Move() {
        move = 0;
    }

    Move(u8 src_square, u8 dest_square, Move_Flag move_flag) {
        // &s with bitstring of desired length to ensure the correct bits are being updated.
        move |= (move_flag & 0x0F);                  // Move flag (4 bits) - bits 0-3
        move |= ((dest_square & 0x3F) << 4);         // Destination square (6 bits) - bits 4-9
        move |= ((src_square & 0x3F) << 10);         // Source square (6 bits) - bits 10-15
    }

    Move_Flag get_flag() const {
        switch(move & 0x0F) {
            case PAWN_FLAG:
                return PAWN_FLAG;
            case KNIGHT_FLAG:
                return KNIGHT_FLAG;
            case BISHOP_FLAG:
                return BISHOP_FLAG;
            case ROOK_FLAG:
                return ROOK_FLAG;
            case QUEEN_FLAG:
                return QUEEN_FLAG;
            case KING_FLAG:
                return KING_FLAG;
            case PAWN_TWO_FORWARD_FLAG:
                return PAWN_TWO_FORWARD_FLAG;
            case EN_PASSANT_FLAG:
                return EN_PASSANT_FLAG;
            case KNIGHT_PROMOTION_FLAG:
                return KNIGHT_PROMOTION_FLAG;
            case BISHOP_PROMOTION_FLAG:
                return BISHOP_PROMOTION_FLAG;
            case ROOK_PROMOTION_FLAG:
                return ROOK_PROMOTION_FLAG;
            case QUEEN_PROMOTION_FLAG:
                return QUEEN_PROMOTION_FLAG;
            case CASTLING_FLAG:
                return CASTLING_FLAG;
            default:
                return NULL_FLAG;  // Return a default value if unknown flag
        }
    }

    Piece get_piece_type_from_flag() { // infers piece type from flag
        switch(move & 0x0F) {
            case PAWN_FLAG:
                return Piece::PAWN;
            case KNIGHT_FLAG:
                return Piece::KNIGHT;
            case BISHOP_FLAG:
                return Piece::BISHOP;
            case ROOK_FLAG:
                return Piece::ROOK;
            case QUEEN_FLAG:
                return Piece::QUEEN;
            case KING_FLAG:
                return Piece::KING;
            case PAWN_TWO_FORWARD_FLAG:
                return Piece::PAWN;
            case EN_PASSANT_FLAG:
                return Piece::PAWN;
            case KNIGHT_PROMOTION_FLAG:
                return Piece::KNIGHT;
            case BISHOP_PROMOTION_FLAG:
                return Piece::BISHOP;
            case ROOK_PROMOTION_FLAG:
                return Piece::ROOK;
            case QUEEN_PROMOTION_FLAG:
                return Piece::QUEEN;
            case CASTLING_FLAG:
                return Piece::KING;
            default:
                return Piece::INVALID;  // Default piece
        }
    }

    u8 get_src_square() {
        return (move >> 10) & 0x3F;
    }

    u8 get_dest_square() {
        return (move >> 4) & 0x3F;
    }

    bool equals(Move& moveb) {
        if(get_src_square() != moveb.get_src_square()) { return false; }
        if(get_dest_square() != moveb.get_dest_square()) { return false; }
        if(get_flag() != moveb.get_flag()) { return false; }
        return true;
    }

};

struct PV
{
    int num_of_moves;
    std::vector<Move> moves;
};

#endif // #ifndef TYPES_HPP