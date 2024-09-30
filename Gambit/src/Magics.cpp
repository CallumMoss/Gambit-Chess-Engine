// #include "Magics.hpp"
// #include "Types.hpp"

// // Credit to https://www.youtube.com/watch?v=_vqlIPDR2TU&t=1s for the inspiration.
// // Used to find bitboards of blocking pieces for sliding pieces
// bb_vector Magics::create_blocker_bitboards(u64 moves) {
//     // Finds a list of the indices for each 1 bit in moves, this ensures that we only consider pieces within vision of the piece.
//     bb_vector move_bits_indices = {};
//     for(int i = 0; i < 64; i++) {
//         if((moves >> i) & 1)
//         {
//             move_bits_indices.push_back(i);
//         }
//     }

//     // Find number of variations of blocker bitboards (2^n)
//     int number_of_variations = move_bits_indices.Count;
//     bb_vector blocker_bitboards = new u64[number_of_variations];

//     // From 0 to the number of possible piece arrangements within vision, and shift them into their proper position on the 64 square bb
//     // The blocking pieces use positioning based on the attacking pieces vision. Therefore square 1 for them is different to square 1 for normal pieces.
//     // We are shifting pieces from their position in the vision bb to their position on the actual bit board.
//     for(int variation_index = 0; variation_index < number_of_variations; variation_index++) {
//         for(int bit_index = 0; bit_index < move_bits_indices; bit_index++) {
//             int bit = ((variation_index >> bit_index) & 1);
//             blocker_bitboards[variation_index] |= bit << move_bits_indeces[bit_index];
//         }
//     }
//     return blocker_bitboards;
// }
