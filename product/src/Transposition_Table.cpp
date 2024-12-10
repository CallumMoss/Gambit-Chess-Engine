#include "Transposition_Table.hpp"

/**
 * @brief adds an entry to transposition table
 * 
 * @param zobrist_key 
 * @param score 
 * @param best_move 
 * @param depth 
 * @param node_type 
 */

void Transposition_Table::add_entry(u64 zobrist_key, int score, Move best_move, int depth, Node_Type node_type) {
    TT_Entry entry(zobrist_key, score, best_move, depth, node_type);
    transposition_table[find_tt_index(zobrist_key)] = entry;
}   

/**
 * @brief resizes the transposition table based on UCI input
 * 
 * @param tt_size_in_mb 
 */
void Transposition_Table::resize(int tt_size_in_mb) {
    number_of_elements = (tt_size_in_mb * 1024 * 1024) / sizeof(TT_Entry); // convert megabytes to bytes and divide by the size of the TT Entry
    transposition_table.reserve(number_of_elements);
}

// Inspired by: https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
/**
 * @brief GCC specific function that finds the index in the transposition table of a position based off its zobrist key.
 * 
 * @param zobrist_key 
 * @return u64 
 */
u64 Transposition_Table::find_tt_index(u64 zobrist_key) {
    return static_cast<u64>((static_cast<unsigned __int128>(zobrist_key) * static_cast<unsigned __int128>(number_of_elements)) >> 64);   
}

/**
 * @brief Replaces all entries in the transposition table with 0
 * 
 */
void Transposition_Table::clear_table() { // used for the clear UCI command when starting a new game, probably helpful in SPRTs
    std::fill_n(transposition_table.begin(), number_of_elements, TT_Entry());
}

// If the entry is already in the transposition table, returns true
bool Transposition_Table::entry_is_in_tt(u64 zobrist_key) {
    if(zobrist_key == transposition_table[find_tt_index(zobrist_key)].zobrist_key) { return true; }
    return false;
}

TT_Entry Transposition_Table::get_entry(u64 zobrist_key) {
    return transposition_table[find_tt_index(zobrist_key)];
}