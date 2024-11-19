#include <iostream>
#include "Position.hpp"
#include "Magics.hpp"
#include "UCI.hpp"

int main() {
    float version = 1.0;

    Magics::init();
    std::string input = "";
    std::string output = "";
// go wtime<>

// go btime<>

// go winc<>

// go binc<>

// Position startpos

// Position fen <fen>

// Create a time management system

    // Inspired by https://github.com/TiltedDFA/TDFA/blob/c26a01e29ba87c41af50700c2c8321e3e2667c8f/src/Uci.cpp
    while(std::getline(std::cin, input)) { // whilst isnt empty
        std::vector<std::string_view> args = split_args(input);
        std::string_view command = args[0];
        // parameters = rest of args
        if(command == "uci") {
            output += "id name Gambit ", version, "\n"; // insert version here
            output += "id author Callum Moss\n";
            // can print option commands
            output += "info string licensed to\n"; // something
            output+= "uciok\n"; // engine has set all parameters and is ready
            std::cout << output;
        }
        else if(command == "isready") {
            // Set time for engine to move per turn?
            std::cout << "readyok\n";
        }
        else if(command == "ucinewgame") {
            Position pos = Position();
        }
        else if(command == "position") {
            // do stuff
        }
        else if (command == "go") {
            // do stuff
        }
        else if(command == "quit") {
            break;
        }
        output = "";
    }

    return 0;
}