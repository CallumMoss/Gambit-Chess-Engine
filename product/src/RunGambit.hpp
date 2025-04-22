#ifndef RUNGAMBIT_HPP
#define RUNGAMBIT_HPP

#include "Types.hpp" // Needed in every file with an assert for assert checker macro
#include "Position.hpp"
#include "Magics.hpp"
#include "UCI.hpp"
#include "Timer.hpp"
#include "Zobrist.hpp"
#include "Transposition_Table.hpp"

#include <iostream>

inline void run_gambit(std::vector<std::string> args = {})
{
    std::string version = "v1.19.1";

    Magics::init();
    Zobrist zobrist;
    Transposition_Table tt;
    tt.resize(16);
    std::string input = "";
    Timer timer;
    Move null_move;
    Position pos = Position();
    PositionStack ps; // vector that stores occured positions and searched positions
    ps.reserve(255);

    bool is_gambit = true; // default gambit search to true
    Opponent opp;

    std::vector<EvaluatedMove> evaluated_opp_responses;

    // Inspired by https://github.com/TiltedDFA/TDFA/blob/c26a01e29ba87c41af50700c2c8321e3e2667c8f/src/Uci.cpp
    if(args.empty())
    {
        while(std::getline(std::cin, input))
        {
            std::vector<std::string> args = UCI::split_args(input);
            std::string command = args[0];
            // parameters = rest of args
            if(command == "uci") {
                std::cout << "id name Gambit " << version << std::endl; // insert version here
                std::cout << "id author Callum Moss" << std::endl;
                std::cout << "uciok" << std::endl; // engine has set all parameters and is ready
            }
            else if(command == "isready") {
                std::cout << "readyok\n";
            }
            else if(command == "ucinewgame") {
                pos = Position();
                tt.clear_table();
                ps.clear();
                opp = Opponent();
            }
            else if (command == "setoption") {
                if(args[1] == "Search" || args[1] == "search") { // setoption Search default || setoption Search Default || setoption Search Gambit || setoption Search gambit
                    if(args[2] == "gambit" || args[2] == "Gambit") {
                        is_gambit = true;
                        std::cout << "Gambit Search = True" << std::endl;
                    }
                    else if(args[2] == "default" || args[2] == "Default")
                    {
                        is_gambit = false;
                        std::cout << "Gambit Search = False" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Invalid search option" << std::endl;
                    }
                }
                else if(args[1] == "Hash") {
                    tt.resize(UCI::options(args));
                }
            }
            else if(command == "position") {
                ps.clear();
                UCI::position(args, pos, ps, is_gambit, opp, evaluated_opp_responses);
            }
            else if (command == "go") {
                UCI::go(args, timer, pos, tt, ps, is_gambit, opp, evaluated_opp_responses);
            }
            else if(command == "quit") {
                break;
            }
        }
    }
    else // used for tests.exe to provide UCI arguments
    {
        for(std::string command : args)
        {
            std::vector<std::string> args = UCI::split_args(input);
            // parameters = rest of args
            if(command == "uci") {
                std::cout << "id name Gambit " << version << std::endl; // insert version here
                std::cout << "id author Callum Moss" << std::endl;
                std::cout << "uciok" << std::endl; // engine has set all parameters and is ready
            }
            else if(command == "isready") {
                std::cout << "readyok\n";
            }
            else if(command == "ucinewgame") {
                pos = Position();
                tt.clear_table();
                ps.clear();
            }
            else if (command == "setoption") {
                if(args[1] == "Search" || args[1] == "search") { // setoption Search default || setoption Search Default || setoption Search Gambit || setoption Search gambit
                    if(args[2] == "gambit" || args[2] == "Gambit") {
                        is_gambit = true;
                        std::cout << "Gambit Search = True" << std::endl;
                    }
                    else if(args[2] == "default" || args[2] == "Default")
                    {
                        is_gambit = false;
                        std::cout << "Gambit Search = False" << std::endl;
                    }
                    else
                    {
                        std::cerr << "Invalid search option" << std::endl;
                    }
                }
                else if(args[1] == "Hash") {
                    tt.resize(UCI::options(args));
                }
            }
            else if(command == "position") {
                ps.clear();
                UCI::position(args, pos, ps, is_gambit, opp, evaluated_opp_responses);
            }
            else if (command == "go") {
                UCI::go(args, timer, pos, tt, ps, is_gambit, opp, evaluated_opp_responses);
            }
            else if(command == "quit") {
                break;
            }
        }
    }
}

#endif // #ifndef RUNGAMBIT_HPP