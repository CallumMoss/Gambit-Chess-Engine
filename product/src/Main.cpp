#include "Types.hpp" // Needed in every file with an assert for assert checker macro
#include "Position.hpp"
#include "Magics.hpp"
#include "UCI.hpp"
#include "Timer.hpp"
#include "Zobrist.hpp"
#include "Transposition_Table.hpp"
#include "Game_History.hpp"

#include <iostream>

int main() {
    std::string version = "v1.9.0";

    Magics::init();
    Zobrist zobrist;
    Transposition_Table tt;
    tt.resize(16);
    std::string input = "";
    std::string output = "";
    Timer timer;
    Move null_move;
    Position pos = Position();
    Game_History gh; // object that stores previously played moves and is updated and reverted during search to check for repetitions

    // Inspired by https://github.com/TiltedDFA/TDFA/blob/c26a01e29ba87c41af50700c2c8321e3e2667c8f/src/Uci.cpp
    while(std::getline(std::cin, input)) { // whilst isnt empty
        std::vector<std::string> args = UCI::split_args(input);
        std::string command = args[0];
        // parameters = rest of args
        if(command == "uci") {
            std::cout << "id name Gambit " << version << std::endl; // insert version here
            std::cout << "id author Callum Moss" << std::endl;
            // can print option commands
            std::cout << "uciok" << std::endl; // engine has set all parameters and is ready
        }
        else if(command == "isready") {
            std::cout << "readyok\n";
        }
        else if(command == "ucinewgame") {
            pos = Position();
            tt.clear_table();
            gh.clear();
        }
        else if (command == "setoption") {
            tt.resize(UCI::options(args));
        }
        else if(command == "position") {
            gh.clear();
            UCI::position(args, pos, gh);
        }
        else if (command == "go") {
            UCI::go(args, timer, pos, tt, gh);
        }
        else if(command == "quit") {
            break;
        }
        output = "";
    }

    return 0;
}