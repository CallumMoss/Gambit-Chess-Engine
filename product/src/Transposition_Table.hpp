#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "Types.hpp"

class Transposition_Table {
    public:
        void add_entry(u64 zobrist_key, int score, Move best_move, int depth, Node_Type node_type);
        void resize(int tt_size_in_mb);
        u64 find_tt_index(u64 key);
        void clear_table();
        bool entry_is_in_tt(u64 zobrist_key);
        bool evaluated_at_same_depth(u64 zobrist_key, int depth);
        TT_Entry get_entry(u64 zobrist_key);

    private:
        std::vector<TT_Entry> transposition_table;
        int number_of_elements = (16 * 1024 * 1024) / sizeof(TT_Entry); // default value is 16 MB
};

#endif // #ifndef TRANSPOSITION_TABLE_HPP