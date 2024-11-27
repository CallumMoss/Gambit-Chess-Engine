#ifndef ZOBRIST_HPP
#define ZOBRIST_HPP

#include "Types.hpp"
#include "utils.hpp"

#include <array>
#include <random>
#include <vector>
#include <format>

// Comment out #define if dont want to use debug mode (for example if want to use for perft)
//#define ZOBRIST_DBG
class Zobrist
{
    #ifdef ZOBRIST_DBG
    public:
        inline static std::vector<std::string> call_order{};
    #endif
    public:
        Zobrist();
        static u64 get_side_to_move()
        {
            #ifdef ZOBRIST_DBG
                call_order.push_back(std::format("ZOBRIST_CALL:\tside_to_move\treturned:{}", instance->side_to_move));
            #endif
            return instance->side_to_move;
        }
        static u64 get_piece(u8 square, int type)
        {
            #ifdef ZOBRIST_DBG
                call_order.push_back(std::format("ZOBRIST_CALL:\tpiece sq:{}, \ttype:{} \treturned:{}", Utils::square_to_board_notation(square), type, instance->pieces[square][type]));
            #endif
            return instance->pieces[square][type];
        }
        static u64 get_en_passant(u8 square)
        {
            #ifdef ZOBRIST_DBG
                call_order.push_back(std::format("ZOBRIST_CALL:\ten_passant sq:{}\treturned:{}", Utils::square_to_board_notation(square), instance->en_passant[square]));
            #endif
            return instance->en_passant[square];
        }
        static u64 get_castling_rights(u8 castling_right)
        {
            #ifdef ZOBRIST_DBG
                call_order.push_back(std::format("ZOBRIST_CALL:\tcastling_rights cr:{}\treturned:{}", castling_right, instance->castling_rights[castling_right]));
            #endif
            return instance->castling_rights[castling_right];
        }
        static void print_log()
        {
            #ifdef ZOBRIST_DBG
            for(auto const& i : call_order)
                std::cout << i << '\n';
            #endif
        }
        static void clear_log()
        {
            #ifdef ZOBRIST_DBG
            call_order.clear();
            #endif
        }
    private:
        // array of 64 arrays of 12 u64s
        //HAS TO BE DONE!!!!!!
        //0-5(inclusive) = white, (6-11) = black
        std::array<std::array<u64, 12>, 64> pieces; // filled with pseudorandom numbers, 64 squares and 12 piece types (white rook, black rook etc). Unique for different piece type on each square
        std::array<u64, 64> en_passant;
        std::array<u64, 16> castling_rights; // stores pseudorandom numbers that represent all combinations of castling rights.
        u64 side_to_move;
        inline static Zobrist* instance = nullptr;
};

#endif // ZOBRIST_HPP