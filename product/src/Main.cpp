#include <iostream>

#include "Position.hpp"
#include "Magics.hpp"
#include "UCI.hpp"
#include "Timer.hpp"


int main() {
    std::string version = "v1.1.0";

    Magics::init();
    std::string input = "";
    std::string output = "";
    Position pos = Position();
    Timer timer;

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
        }
        else if(command == "position") {
            UCI::position(args, pos);
        }
        else if (command == "go") {
            UCI::go(args, timer, pos);
        }
        else if(command == "quit") {
            break;
        }
        output = "";
    }

    return 0;
}