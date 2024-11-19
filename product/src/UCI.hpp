#ifndef UCI_HPP
#define UCI_HPP

#include <chrono>

#include "Types.hpp"
#include "Position.hpp"

std::vector<std::string_view> split_args(std::string_view input)
{
    std::vector<std::string_view> args;

    if(input.size() == 0 ) return {""};

    std::size_t start{0}, end{0};

    while(end < input.size())
    {
        if(input[end++] == ' ')
        {
            args.push_back(input.substr(start, end - start - 1));
            start = end;
        }
    }

    if(input[input.size() - 1] != ' ')
        args.push_back(input.substr(start, end - start));

    return args;
}

// Helpful comment from: https://github.com/AndyGrant/Ethereal/blob/master/src/uci.c
    /*
    |------------|-----------------------------------------------------------------------|
    |  Commands  | Response. * denotes that the command blocks until no longer searching |
    |------------|-----------------------------------------------------------------------|
    |        uci |           Outputs the engine name, authors, and all available options |
    |    isready | *           Responds with readyok when no longer searching a position |
    | ucinewgame | *  Resets the TT and any Hueristics to ensure determinism in searches |
    |  setoption | *     Sets a given option and reports that the option was set if done |
    |   position | *  Sets the board position via an optional FEN and optional move list |
    |         go | *       Searches the current position with the provided time controls |
    |  ponderhit |          Flags the search to indicate that the ponder move was played |
    |       stop |            Signals the search threads to finish and report a bestmove |
    |       quit |             Exits the engine and any searches by killing the UCI loop |
    |      perft |            Custom command to compute PERFT(N) of the current position |
    |      print |         Custom command to print an ASCII view of the current position |
    |------------|-----------------------------------------------------------------------|
    */

// Inspired by https://github.com/AndyGrant/Ethereal/blob/master/src/uci.c#L133
void uci_go(std::vector<std::string_view> args) {
    auto start = std::chrono::high_resolution_clock::now();

    u64 wtime = 0; // white time (for example in 10+0, may be 10mins, or might be time left?)
    u64 btime = 0;
    u64 winc = 0; // white increment
    u64 binc = 0;

    //std::cout << "bestmove" << find_best_move() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

//         for(size_t i{0}; i < args.size(); ++i)
//         {
//             if(args[i] == "wtime")
//                 std::from_chars(args[i + 1].data(), args[i + 1].data() + args[i + 1].size(), wtime);
//             if(args[i] == "btime")
//                 std::from_chars(args[i + 1].data(), args[i + 1].data() + args[i + 1].size(), btime);
//             if(args[i] == "winc")
//                 std::from_chars(args[i + 1].data(), args[i + 1].data() + args[i + 1].size(), binc);
//             if(args[i] == "binc")
//                 std::from_chars(args[i + 1].data(), args[i + 1].data() + args[i + 1].size(), binc);
//         }
//         if(pos.WhiteToMove())
//         {
//             time_manager.SetOptions(wtime, winc);
//         }
//         else
//         {
//             time_manager.SetOptions(btime, binc);
//         }
//     }
//     //start the timer for this round of calculation
//     time_manager.StartTiming();
//     std::cout << std::format("bestmove {}\n", UTIL::MoveToStr(Search::FindBestMove(&pos, &transpos_table, &time_manager)));
// }

void uci_position() {
    // if() {
    //     ;
    // }
}

#endif // #ifndef UCI_HPP